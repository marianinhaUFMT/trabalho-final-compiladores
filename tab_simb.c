#include <stdio.h>
#include <string.h>
#include "tab_simb.h"

TabelaVariaveis ts_global;
TabelaVariaveis ts_local;
TabelaFuncoes   ts_funcoes;
SimboloFuncao  *funcao_atual = NULL;
int             dentro_de_funcao = 0;

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
        return 0; /* ja existe nesta TS: redeclaracao */
    }
    strncpy(ts->vars[ts->n].nome, nome, MAX_NOME - 1);
    ts->vars[ts->n].nome[MAX_NOME - 1] = '\0';
    ts->vars[ts->n].tipo = tipo;
    ts->vars[ts->n].inicializada = 0;
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