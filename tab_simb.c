#include <stdio.h>
#include <string.h>
#include "tab_simb.h"

TabelaVariaveis ts_global;
TabelaVariaveis ts_local;
TabelaFuncoes   ts_funcoes;
SimboloFuncao  *funcao_atual = NULL;
int             dentro_de_funcao = 0;
int             inserindo_parametro = 0;

const char* tipo_para_str(Tipo t) {
    switch (t) {
        case T_INT:    return "int";
        case T_FLOAT:  return "float";
        case T_CHAR:   return "char";
        case T_BOOL:   return "bool";
        case T_STRING: return "string";
        default:       return "indefinido";
    }
}

/* ---------------- variaveis ---------------- */

void ts_var_init(TabelaVariaveis *ts) {
    ts->n = 0;
}

int ts_var_insere(TabelaVariaveis *ts, const char *nome, Tipo tipo) {
    if (ts_var_busca(ts, nome) != NULL) {
        return 0; /* já existe: erro de redeclaração */
    }
    
    int idx = ts->n;
    strncpy(ts->vars[idx].nome, nome, MAX_NOME - 1);
    ts->vars[idx].nome[MAX_NOME - 1] = '\0';
    ts->vars[idx].tipo = tipo;
    ts->vars[idx].inicializada = 0;

    // Se estivermos manipulando a tabela local (ts_local)
    if (ts == &ts_local) {
        if (inserindo_parametro) {
            /* Parâmetros em x86 entram empilhados antes do EBP.
               O primeiro parâmetro fica em [ebp+8], o segundo em [ebp+12], etc. */
            ts->vars[idx].offset = 8 + (idx * 4);
        } else {
            /* Variáveis locais ficam abaixo do EBP (valores negativos).
               Se a função já tem parâmetros inseridos antes, precisamos desconsiderar 
               a contagem de parâmetros para calcular o offset local. */
            int n_locais_puras = 0;
            for(int i = 0; i < idx; i++) {
                if(ts->vars[i].offset < 0) n_locais_puras++;
            }
            ts->vars[idx].offset = -4 - (n_locais_puras * 4);
        }
    } else {
        ts->vars[idx].offset = 0; // Globais usam rótulos de memória, não offsets
    }

    ts->n++;
    return 1;
}

SimboloVar* ts_var_busca(TabelaVariaveis *ts, const char *nome) {
    for (int i = 0; i < ts->n; i++) {
        if (strcmp(ts->vars[i].nome, nome) == 0) {
            return &ts->vars[i];
        }
    }
    return NULL;
}

SimboloVar* busca_variavel(const char *nome) {
    /* prioridade: TS local, depois TS global */
    SimboloVar *v = ts_var_busca(&ts_local, nome);
    if (v != NULL) return v;
    return ts_var_busca(&ts_global, nome);
}

void marca_inicializada(const char *nome) {
    SimboloVar *v = busca_variavel(nome);
    if (v != NULL) v->inicializada = 1;
}

/* ---------------- funcoes ---------------- */

void ts_func_init(TabelaFuncoes *ts) {
    ts->n = 0;
}

SimboloFuncao* ts_func_busca(const char *nome) {
    for (int i = 0; i < ts_funcoes.n; i++) {
        if (strcmp(ts_funcoes.funcs[i].nome, nome) == 0) {
            return &ts_funcoes.funcs[i];
        }
    }
    return NULL;
}

SimboloVar* busca_variavel_info(const char *nome, int *is_local) {
    // Procura primeiro na tabela local
    SimboloVar *v = ts_var_busca(&ts_local, nome);
    if (v != NULL) {
        *is_local = 1;
        return v;
    }
    // Se não achar, procura na global
    v = ts_var_busca(&ts_global, nome);
    if (v != NULL) {
        *is_local = 0;
        return v;
    }
    return NULL; // Não encontrada
}


SimboloFuncao* ts_func_insere_prototipo(const char *nome, Tipo tipo_retorno) {
    SimboloFuncao *existente = ts_func_busca(nome);
    if (existente != NULL) {
        return existente;
    }
    SimboloFuncao *f = &ts_funcoes.funcs[ts_funcoes.n];
    strncpy(f->nome, nome, MAX_NOME - 1);
    f->nome[MAX_NOME - 1] = '\0';
    f->tipo_retorno = tipo_retorno;
    f->n_params = 0;
    f->prototipada = 0;
    f->implementada = 0;
    f->tem_return = 0;
    ts_funcoes.n++;
    return f;
}

void ts_func_adiciona_param(SimboloFuncao *f, const char *nome, Tipo tipo) {
    if (f->n_params >= MAX_PARAMS) return;
    strncpy(f->params[f->n_params].nome, nome, MAX_NOME - 1);
    f->params[f->n_params].nome[MAX_NOME - 1] = '\0';
    f->params[f->n_params].tipo = tipo;
    f->n_params++;
}

int tamanho_locais_atual(void) {
    int total = 0;
    for (int i = 0; i < ts_local.n; i++) {
        // Apenas variáveis com offset negativo contam como locais que exigem 'sub esp, X'
        if (ts_local.vars[i].offset < 0) {
            total += 4; // Cada variável ocupa 4 bytes (32-bits)
        }
    }
    return total;
}

/* ---------------- impressao final ---------------- */

void imprime_tabelas(void) {
    printf("\n==================== TABELA DE SIMBOLOS - VARIAVEIS GLOBAIS ====================\n");
    printf("%-20s %-10s\n", "NOME", "TIPO");
    for (int i = 0; i < ts_global.n; i++) {
        printf("%-20s %-10s\n", ts_global.vars[i].nome, tipo_para_str(ts_global.vars[i].tipo));
    }

    printf("\n==================== TABELA DE SIMBOLOS - FUNCOES ====================\n");
    printf("%-20s %-10s %-s\n", "NOME", "RETORNO", "PARAMETROS");
    for (int i = 0; i < ts_funcoes.n; i++) {
        SimboloFuncao *f = &ts_funcoes.funcs[i];
        printf("%-20s %-10s (", f->nome, tipo_para_str(f->tipo_retorno));
        for (int j = 0; j < f->n_params; j++) {
            printf("%s %s%s", tipo_para_str(f->params[j].tipo), f->params[j].nome,
                   (j < f->n_params - 1) ? ", " : "");
        }
        printf(")\n");
    }
    printf("\n");
}