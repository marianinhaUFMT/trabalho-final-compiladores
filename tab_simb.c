#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_simb.h"

Escopo        *escopo_global = NULL;
Escopo        *escopo_atual = NULL;
TabelaFuncoes  ts_funcoes;
SimboloFuncao *funcao_atual = NULL;
int            dentro_de_funcao = 0;
int            inserindo_parametro = 0;

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

/* ---------------- Gerenciamento do Encadeamento de Escopos ---------------- */

Escopo* criar_escopo(Escopo *pai) {
    Escopo *novo = (Escopo*) malloc(sizeof(Escopo));
    if (!novo) {
        fprintf(stderr, "[ERRO COMPILADOR] Falha na alocação de memória para Escopo.\n");
        exit(EXIT_FAILURE);
    }
    novo->n = 0;
    novo->pai = pai; /* Estabelece a ligação na cadeia de escopos */
    return novo;
}

void entra_escopo(void) {
    escopo_atual = criar_escopo(escopo_atual);
}

void sai_escopo(void) {
    if (escopo_atual != NULL && escopo_atual->pai != NULL) {
        Escopo *velho = escopo_atual;
        escopo_atual = escopo_atual->pai; /* Sobe um nível no encadeamento */
        free(velho);
    }
}

/* ---------------- Operações com Variáveis ---------------- */

SimboloVar* ts_var_busca_no_escopo(Escopo *e, const char *nome) {
    if (e == NULL) return NULL;
    for (int i = 0; i < e->n; i++) {
        if (strcmp(e->vars[i].nome, nome) == 0) {
            return &e->vars[i];
        }
    }
    return NULL;
}

int ts_var_insere(Escopo *e, const char *nome, Tipo tipo) {
    if (e == NULL) e = escopo_atual;

    /* Redeclaração no MESMO escopo é proibida */
    if (ts_var_busca_no_escopo(e, nome) != NULL) {
        return 0; 
    }
    
    int idx = e->n;
    strncpy(e->vars[idx].nome, nome, MAX_NOME - 1);
    e->vars[idx].nome[MAX_NOME - 1] = '\0';
    e->vars[idx].tipo = tipo;
    e->vars[idx].inicializada = 0;

    if (e != escopo_global) {
        if (inserindo_parametro) {
            /* Parâmetros em x86: empilhados antes do EBP ([ebp+8], [ebp+12], ...) */
            e->vars[idx].offset = 8 + (idx * 4);
        } else {
            /* Variáveis locais: abaixo do EBP com valores negativos ([ebp-4], [ebp-8], ...) */
            int n_locais_puras = 0;
            for (int i = 0; i < idx; i++) {
                if (e->vars[i].offset < 0) n_locais_puras++;
            }
            e->vars[idx].offset = -4 - (n_locais_puras * 4);
        }
    } else {
        e->vars[idx].offset = 0; /* Variáveis globais utilizam rótulos na seção .data */
    }

    e->n++;
    return 1;
}

/* Busca Encadeada: Navega do escopo ativo subindo pelos ponteiros 'pai' até o global */
SimboloVar* busca_variavel_info(const char *nome, int *is_local) {
    Escopo *aux = escopo_atual;
    while (aux != NULL) {
        SimboloVar *v = ts_var_busca_no_escopo(aux, nome);
        if (v != NULL) {
            if (is_local) {
                *is_local = (aux != escopo_global);
            }
            return v;
        }
        aux = aux->pai; /* Passo do encadeamento: sobe um nível na hierarquia */
    }
    return NULL;
}

SimboloVar* busca_variavel(const char *nome) {
    int is_local;
    return busca_variavel_info(nome, &is_local);
}

void marca_inicializada(const char *nome) {
    SimboloVar *v = busca_variavel(nome);
    if (v != NULL) v->inicializada = 1;
}

int tamanho_locais_atual(void) {
    int total = 0;
    /* Calcula o espaço local necessário para o escopo local ativo */
    if (escopo_atual != NULL && escopo_atual != escopo_global) {
        for (int i = 0; i < escopo_atual->n; i++) {
            if (escopo_atual->vars[i].offset < 0) {
                total += 4;
            }
        }
    }
    return total;
}

/* ---------------- Tabela de Funções ---------------- */

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

/* ---------------- Impressão de Relatórios ---------------- */

void imprime_tabelas(void) {
    printf("\n==================== TABELA DE SIMBOLOS - VARIAVEIS GLOBAIS ====================\n");
    printf("%-20s %-10s\n", "NOME", "TIPO");
    if (escopo_global != NULL) {
        for (int i = 0; i < escopo_global->n; i++) {
            printf("%-20s %-10s\n", escopo_global->vars[i].nome, tipo_para_str(escopo_global->vars[i].tipo));
        }
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