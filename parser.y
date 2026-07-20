%code requires {
    #include "tab_simb.h"
    #include "codeGeneration.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_simb.h"
#include "codeGeneration.h"

int  yylex(void);
void yyerror(const char *s);

extern int linha_atual;
extern int inserindo_parametro;

int erros_encontrados = 0;
int main_definida = 0;

FILE *output_file;
static int secao_data_aberta = 0;
static int secao_text_aberta = 0;

/* acumulador temporario usado para montar a assinatura de uma funcao
   (prototipo ou implementacao) antes de gravar/validar na TS de funcoes */
static SimboloFuncao temp_func;

/* acumulador temporario dos argumentos de uma chamada de funcao (tipo + codigo gerado) */
static char  chamada_nome[MAX_NOME];
static Tipo  chamada_args[MAX_PARAMS];
static char  temp_arg_codes[MAX_PARAMS][CODE_BUF];
static int   chamada_n_args;

static void erro_semantico(const char *msg) {
    fprintf(stderr, "[ERRO SEMANTICO] linha %d: %s\n", linha_atual, msg);
    erros_encontrados++;
}

/* compatibilidade simples entre tipos (int/float sao mutuamente compativeis) */
static int compativel(Tipo a, Tipo b) {
    if (a == b) return 1;
    if ((a == T_INT || a == T_FLOAT) && (b == T_INT || b == T_FLOAT)) return 1;
    return 0;
}

static void inicia_chamada(const char *nome) {
    strncpy(chamada_nome, nome, MAX_NOME - 1);
    chamada_nome[MAX_NOME - 1] = '\0';
    chamada_n_args = 0;
}

static void empilha_arg(Tipo t, char *code) {
    if (chamada_n_args < MAX_PARAMS) {
        chamada_args[chamada_n_args] = t;
        strcpy(temp_arg_codes[chamada_n_args], code);
        chamada_n_args++;
    }
}

static Tipo finaliza_chamada(void) {
    SimboloFuncao *f = ts_func_busca(chamada_nome);
    if (f == NULL || !f->prototipada) {
        char msg[512];
        sprintf(msg, "chamada da funcao '%s' nao declarada (sem prototipo)", chamada_nome);
        erro_semantico(msg);
        return T_INDEFINIDO;
    }
    if (chamada_n_args != f->n_params) {
        char msg[512];
        sprintf(msg, "funcao '%s' espera %d parametro(s), mas recebeu %d",
                chamada_nome, f->n_params, chamada_n_args);
        erro_semantico(msg);
        return f->tipo_retorno;
    }
    for (int i = 0; i < chamada_n_args; i++) {
        if (!compativel(chamada_args[i], f->params[i].tipo)) {
            char msg[512];
            sprintf(msg, "parametro %d da chamada de '%s' incompativel: esperado %s, recebido %s",
                    i + 1, chamada_nome, tipo_para_str(f->params[i].tipo), tipo_para_str(chamada_args[i]));
            erro_semantico(msg);
        }
    }
    return f->tipo_retorno;
}
%}

%union {
    struct {
        char str[256];
        Tipo tipo;
        char code[CODE_BUF];
    } c;
}

%token <c> INT FLOAT CHAR BOOL STRING
%token <c> VARIABLE INTEGER FLOAT_CONST CHAR_CONST STRING_CONST
%token IF ELSE WHILE READ WRITE RETURN TRUE FALSE MAIN
%token EQ NE LT LE GT GE NOT

%type <c> tipo expr chamada_funcao comando lista_comandos
%type <c> declaracao_variavel atribuicao condicional laco leitura escrita comando_return

%left EQ NE
%left LT GT LE GE
%left '+' '-'
%left '*' '/'
%right NOT

%%

programa:
        lista_declaracoes_globais funcao_main
    ;

lista_declaracoes_globais:
        lista_declaracoes_globais declaracao_global
    |   /* vazio */
    ;

declaracao_global:
        declaracao_variavel
    |   prototipo_funcao
    |   definicao_funcao
    ;

tipo:
        INT     { $$.tipo = T_INT; }
    |   FLOAT   { $$.tipo = T_FLOAT; }
    |   CHAR    { $$.tipo = T_CHAR; }
    |   BOOL    { $$.tipo = T_BOOL; }
    |   STRING  { $$.tipo = T_STRING; }
    ;

/* ---------------- declaracao de variaveis ---------------- */

declaracao_variavel:
        tipo VARIABLE ';'
        {
            $$.code[0] = '\0';
            TabelaVariaveis *ts = dentro_de_funcao ? &ts_local : &ts_global;
            if (!ts_var_insere(ts, $2.str, $1.tipo)) {
                char msg[512];
                sprintf(msg, "variavel '%s' ja declarada neste escopo", $2.str);
                erro_semantico(msg);
            } else if (!dentro_de_funcao) {
                if (!secao_data_aberta) { fprintf(output_file, "section .data\n"); secao_data_aberta = 1; }
                char buf[128]; buf[0] = '\0';
                makeCodeDeclGlobal(buf, $2.str);
                fprintf(output_file, "%s", buf);
            }
            /* variavel local: nao gera codigo aqui, apenas reserva espaco via offset na TS */
        }
    ;

/* ---------------- assinatura compartilhada (prototipo e implementacao) ----------------
   Unificada em uma unica regra para nao gerar conflito reduce/reduce: prototipo e
   implementacao compartilham o mesmo prefixo "tipo VARIABLE ( lista_params )" e so se
   distinguem pelo token seguinte (';' ou '{'). Se cada um tivesse sua propria acao
   embutida logo apos '(' o parser LALR nao teria como decidir qual delas reduzir
   naquele ponto (lookahead insuficiente). */

assinatura_funcao:
        tipo VARIABLE '('
        {
            strncpy(temp_func.nome, $2.str, MAX_NOME - 1);
            temp_func.nome[MAX_NOME - 1] = '\0';
            temp_func.tipo_retorno = $1.tipo;
            temp_func.n_params = 0;
        }
        lista_params_opt ')'
    ;

lista_params_opt:
        lista_params
    |   /* vazio */
    ;

lista_params:
        lista_params ',' tipo VARIABLE
        {
            ts_func_adiciona_param(&temp_func, $4.str, $3.tipo);
        }
    |   tipo VARIABLE
        {
            ts_func_adiciona_param(&temp_func, $2.str, $1.tipo);
        }
    ;

/* ---------------- prototipo de funcao ---------------- */

prototipo_funcao:
        assinatura_funcao ';'
        {
            SimboloFuncao *existente = ts_func_busca(temp_func.nome);
            if (existente != NULL && existente->prototipada) {
                char msg[512];
                sprintf(msg, "funcao '%s' ja possui prototipo (redefinicao nao permitida)", temp_func.nome);
                erro_semantico(msg);
            } else {
                SimboloFuncao *f = ts_func_insere_prototipo(temp_func.nome, temp_func.tipo_retorno);
                f->tipo_retorno = temp_func.tipo_retorno;
                f->n_params = 0;
                for (int i = 0; i < temp_func.n_params; i++) {
                    ts_func_adiciona_param(f, temp_func.params[i].nome, temp_func.params[i].tipo);
                }
                f->prototipada = 1;
            }
        }
    ;

/* ---------------- implementacao de funcao ---------------- */

definicao_funcao:
        assinatura_funcao
        {
            SimboloFuncao *f = ts_func_busca(temp_func.nome);
            if (f == NULL || !f->prototipada) {
                char msg[512];
                sprintf(msg, "funcao '%s' implementada sem prototipo previo", temp_func.nome);
                erro_semantico(msg);
                f = ts_func_insere_prototipo(temp_func.nome, temp_func.tipo_retorno);
                f->n_params = 0;
                for (int i = 0; i < temp_func.n_params; i++) {
                    ts_func_adiciona_param(f, temp_func.params[i].nome, temp_func.params[i].tipo);
                }
                f->prototipada = 1;
            } else {
                int assinatura_ok = (f->tipo_retorno == temp_func.tipo_retorno) &&
                                     (f->n_params == temp_func.n_params);
                if (assinatura_ok) {
                    for (int i = 0; i < f->n_params; i++) {
                        if (f->params[i].tipo != temp_func.params[i].tipo) { assinatura_ok = 0; break; }
                    }
                }
                if (!assinatura_ok) {
                    char msg[512];
                    sprintf(msg, "implementacao de '%s' nao corresponde ao prototipo declarado", temp_func.nome);
                    erro_semantico(msg);
                }
                if (f->implementada) {
                    char msg[512];
                    sprintf(msg, "funcao '%s' ja foi implementada (redefinicao nao permitida)", temp_func.nome);
                    erro_semantico(msg);
                }
            }

            f->implementada = 1;
            funcao_atual = f;
            dentro_de_funcao = 1;
            ts_var_init(&ts_local);
            /* parametros da funcao se comportam como variaveis locais, ja inicializadas */
            inserindo_parametro = 1;
            for (int i = 0; i < temp_func.n_params; i++) {
                ts_var_insere(&ts_local, temp_func.params[i].nome, temp_func.params[i].tipo);
                SimboloVar *pv = ts_var_busca(&ts_local, temp_func.params[i].nome);
                if (pv) pv->inicializada = 1;
            }
            inserindo_parametro = 0;
        }
        '{' lista_comandos '}'
        {
            if (!funcao_atual->tem_return) {
                char msg[512];
                sprintf(msg, "funcao '%s' nao possui comando 'return' (retorno obrigatorio)", funcao_atual->nome);
                erro_semantico(msg);
            }
            if (!secao_text_aberta) { fprintf(output_file, "section .text\n"); secao_text_aberta = 1; }
            char prolog[CODE_BUF]; char epilog[CODE_BUF];
            makeCodeFuncProlog(prolog, funcao_atual->nome, tamanho_locais_atual());
            epilog[0] = '\0';
            fprintf(output_file, "%s", prolog);
            fprintf(output_file, "%s", $4.code);
            fprintf(output_file, "%s", epilog);
            dentro_de_funcao = 0;
            funcao_atual = NULL;
        }
    ;

/* ---------------- funcao main ---------------- */

funcao_main:
        INT MAIN '(' ')'
        {
            if (main_definida) {
                erro_semantico("funcao 'main' ja foi definida");
            }
            main_definida = 1;
            SimboloFuncao *f = ts_func_insere_prototipo("main", T_INT);
            f->prototipada = 1;
            f->implementada = 1;
            funcao_atual = f;
            dentro_de_funcao = 1;
            ts_var_init(&ts_local);
        }
        '{' lista_comandos '}'
        {
            if (!funcao_atual->tem_return) {
                erro_semantico("funcao 'main' nao possui comando 'return' (retorno obrigatorio)");
            }
            if (!secao_text_aberta) { fprintf(output_file, "section .text\n"); secao_text_aberta = 1; }
            fprintf(output_file, "global main\n");
            char prolog[CODE_BUF]; char epilog[CODE_BUF];
            makeCodeFuncProlog(prolog, "main", tamanho_locais_atual());
            epilog[0] = '\0';
            fprintf(output_file, "%s", prolog);
            fprintf(output_file, "%s", $7.code);
            fprintf(output_file, "%s", epilog);
            dentro_de_funcao = 0;
            funcao_atual = NULL;
        }
    ;

/* ---------------- comandos ---------------- */

lista_comandos:
        lista_comandos comando
        {
            strcpy($$.code, $1.code);
            strcat($$.code, $2.code);
        }
    |   /* vazio */ { $$.code[0] = '\0'; }
    ;

comando:
        declaracao_variavel { strcpy($$.code, $1.code); }
    |   atribuicao           { strcpy($$.code, $1.code); }
    |   condicional          { strcpy($$.code, $1.code); }
    |   laco                 { strcpy($$.code, $1.code); }
    |   leitura               { strcpy($$.code, $1.code); }
    |   escrita                { strcpy($$.code, $1.code); }
    |   comando_return          { strcpy($$.code, $1.code); }
    |   chamada_funcao ';'
        {
            strcpy($$.code, $1.code);
            /* não precisa fazer nada com o eax se o retorno for ignorado */
        }
    ;

atribuicao:
        VARIABLE '=' expr ';'
        {
            $$.code[0] = '\0';
            int is_local;
            SimboloVar *v = busca_variavel_info($1.str, &is_local);
            if (v == NULL) {
                char msg[512];
                sprintf(msg, "variavel '%s' nao declarada", $1.str);
                erro_semantico(msg);
            } else if (!compativel(v->tipo, $3.tipo)) {
                char msg[512];
                sprintf(msg, "atribuicao incompativel: variavel '%s' e do tipo %s, mas expressao e do tipo %s",
                        $1.str, tipo_para_str(v->tipo), tipo_para_str($3.tipo));
                erro_semantico(msg);
            } else {
                v->inicializada = 1;
                if (is_local) makeCodeAssignLocal($$.code, $3.code, $1.str, v->offset);
                else           makeCodeAssignGlobal($$.code, $3.code, $1.str);
            }
        }
    ;

condicional:
        IF '(' expr ')' '{' lista_comandos '}'
        {
            $$.code[0] = '\0';
            int l1 = novoLabel(); int l2 = novoLabel();
            makeCodeIf($$.code, $3.code, $6.code, "", 0, l1, l2);
        }
    |   IF '(' expr ')' '{' lista_comandos '}' ELSE '{' lista_comandos '}'
        {
            $$.code[0] = '\0';
            int l1 = novoLabel(); int l2 = novoLabel();
            makeCodeIf($$.code, $3.code, $6.code, $10.code, 1, l1, l2);
        }
    ;

laco:
        WHILE '(' expr ')' '{' lista_comandos '}'
        {
            $$.code[0] = '\0';
            int l1 = novoLabel(); int l2 = novoLabel();
            makeCodeWhile($$.code, $3.code, $6.code, l1, l2);
        }
    ;

leitura:
        READ '(' VARIABLE ')' ';'
        {
            $$.code[0] = '\0';
            int is_local;
            SimboloVar *v = busca_variavel_info($3.str, &is_local);
            if (v == NULL) {
                char msg[512];
                sprintf(msg, "variavel '%s' nao declarada", $3.str);
                erro_semantico(msg);
            } else {
                v->inicializada = 1;
                makeCodeRead($$.code, $3.str, is_local, v->offset);
            }
        }
    ;

escrita:
        WRITE '(' VARIABLE ')' ';'
        {
            $$.code[0] = '\0';
            int is_local;
            SimboloVar *v = busca_variavel_info($3.str, &is_local);
            if (v == NULL) {
                char msg[512];
                sprintf(msg, "variavel '%s' nao declarada", $3.str);
                erro_semantico(msg);
            } else {
                char val[CODE_BUF]; val[0] = '\0';
                if (is_local) makeCodeLoadVarLocal(val, $3.str, v->offset);
                else           makeCodeLoadVarGlobal(val, $3.str);
                makeCodeWrite($$.code, val, (v->tipo == T_STRING));
            }
        }
    |   WRITE '(' INTEGER ')' ';'
        {
            char val[CODE_BUF]; val[0] = '\0';
            makeCodeLoadConst(val, $3.str);
            $$.code[0] = '\0';
            makeCodeWrite($$.code, val, 0);
        }
    |   WRITE '(' FLOAT_CONST ')' ';'
        {
            char val[CODE_BUF]; val[0] = '\0';
            makeCodeLoadConst(val, $3.str);
            $$.code[0] = '\0';
            makeCodeWrite($$.code, val, 0);
        }
    |   WRITE '(' STRING_CONST ')' ';'
        {
            $$.code[0] = '\0';
            int l_str = novoLabel();
            makeCodeWriteStringLiteral($$.code, $3.str, l_str);
        }

comando_return:
        RETURN expr ';'
        {
            $$.code[0] = '\0';
            if (funcao_atual == NULL) {
                erro_semantico("comando 'return' fora do escopo de uma funcao");
            } else {
                funcao_atual->tem_return = 1;
                if (!compativel(funcao_atual->tipo_retorno, $2.tipo)) {
                    char msg[512];
                    sprintf(msg, "tipo de retorno incompativel: funcao '%s' retorna %s, mas expressao e do tipo %s",
                            funcao_atual->nome, tipo_para_str(funcao_atual->tipo_retorno), tipo_para_str($2.tipo));
                    erro_semantico(msg);
                }
                makeCodeReturn($$.code, $2.code);
            }
        }
    ;

/* ---------------- chamada de funcao ---------------- */

chamada_funcao:
        VARIABLE '(' { inicia_chamada($1.str); } lista_args_opt ')'
        {
            $$.tipo = finaliza_chamada();
            strncpy($$.str, chamada_nome, sizeof($$.str) - 1);
            $$.code[0] = '\0';
            for (int i = chamada_n_args - 1; i >= 0; i--) {
                strcat($$.code, temp_arg_codes[i]);
            }
            makeCodeCallEnd($$.code, chamada_nome, chamada_n_args);
        }
    ;

lista_args_opt:
        lista_args
    |   /* vazio */
    ;

lista_args:
        lista_args ',' expr 
        { 
            char arg_code[CODE_BUF];
            arg_code[0] = '\0';
            makeCodeCallPushArg(arg_code, $3.code);
            empilha_arg($3.tipo, arg_code); 
        }
    |   expr                
        { 
            char arg_code[CODE_BUF];
            arg_code[0] = '\0';
            makeCodeCallPushArg(arg_code, $1.code);
            empilha_arg($1.tipo, arg_code); 
        }
    ;

/* ---------------- expressoes ---------------- */

expr:
        INTEGER            { $$.tipo = T_INT; makeCodeLoadConst($$.code, $1.str); }
    |   FLOAT_CONST         { $$.tipo = T_FLOAT; makeCodeLoadConst($$.code, $1.str); }
    |   CHAR_CONST          {
                                $$.tipo = T_CHAR;
                                char numlit[8];
                                sprintf(numlit, "%d", (int)$1.str[0]);
                                makeCodeLoadConst($$.code, numlit);
                            }
    |   STRING_CONST        { $$.tipo = T_STRING; $$.code[0] = '\0'; /* strings: sem codegen ainda (ver limitacao) */ }
    |   TRUE                { $$.tipo = T_BOOL; makeCodeLoadConst($$.code, "1"); }
    |   FALSE               { $$.tipo = T_BOOL; makeCodeLoadConst($$.code, "0"); }
    |   VARIABLE
        {
            int is_local;
            SimboloVar *v = busca_variavel_info($1.str, &is_local);
            if (v == NULL) {
                char msg[512];
                sprintf(msg, "variavel '%s' nao declarada", $1.str);
                erro_semantico(msg);
                $$.tipo = T_INDEFINIDO;
                $$.code[0] = '\0';
            } else {
                $$.tipo = v->tipo;
                if (is_local) makeCodeLoadVarLocal($$.code, $1.str, v->offset);
                else           makeCodeLoadVarGlobal($$.code, $1.str);
            }
        }
    |   chamada_funcao      { $$.tipo = $1.tipo; strcpy($$.code, $1.code); }
    |   expr '+' expr       { $$.tipo = compativel($1.tipo, $3.tipo) ? $1.tipo : T_INDEFINIDO;
                               if ($$.tipo == T_INDEFINIDO) erro_semantico("operandos de '+' com tipos incompativeis");
                               strcpy($$.code, $1.code); makeCodeAdd($$.code, $3.code); }
    |   expr '-' expr       { $$.tipo = compativel($1.tipo, $3.tipo) ? $1.tipo : T_INDEFINIDO;
                               if ($$.tipo == T_INDEFINIDO) erro_semantico("operandos de '-' com tipos incompativeis");
                               strcpy($$.code, $1.code); makeCodeSub($$.code, $3.code); }
    |   expr '*' expr       { $$.tipo = compativel($1.tipo, $3.tipo) ? $1.tipo : T_INDEFINIDO;
                               if ($$.tipo == T_INDEFINIDO) erro_semantico("operandos de '*' com tipos incompativeis");
                               strcpy($$.code, $1.code); makeCodeMul($$.code, $3.code); }
    |   expr '/' expr       { $$.tipo = compativel($1.tipo, $3.tipo) ? $1.tipo : T_INDEFINIDO;
                               if ($$.tipo == T_INDEFINIDO) erro_semantico("operandos de '/' com tipos incompativeis");
                               strcpy($$.code, $1.code); makeCodeDiv($$.code, $3.code); }
    |   expr EQ expr        { $$.tipo = T_BOOL; strcpy($$.code, $1.code); makeCodeRel($$.code, $3.code, "=="); }
    |   expr NE expr        { $$.tipo = T_BOOL; strcpy($$.code, $1.code); makeCodeRel($$.code, $3.code, "!="); }
    |   expr LT expr        { $$.tipo = T_BOOL; strcpy($$.code, $1.code); makeCodeRel($$.code, $3.code, "<"); }
    |   expr LE expr        { $$.tipo = T_BOOL; strcpy($$.code, $1.code); makeCodeRel($$.code, $3.code, "<="); }
    |   expr GT expr        { $$.tipo = T_BOOL; strcpy($$.code, $1.code); makeCodeRel($$.code, $3.code, ">"); }
    |   expr GE expr        { $$.tipo = T_BOOL; strcpy($$.code, $1.code); makeCodeRel($$.code, $3.code, ">="); }
    |   NOT expr            { $$.tipo = T_BOOL; strcpy($$.code, $2.code); makeCodeNot($$.code); }
    |   '(' expr ')'        { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[ERRO SINTATICO] linha %d: %s\n", linha_atual, s);
    erros_encontrados++;
}