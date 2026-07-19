#ifndef TAB_SIMB_H
#define TAB_SIMB_H

#define MAX_NOME    64
#define MAX_VARS    200
#define MAX_FUNCS   100
#define MAX_PARAMS  20

/* ---------- Tipos ---------- */
typedef enum {
    T_INT,
    T_FLOAT,
    T_CHAR,
    T_BOOL,
    T_STRING,
    T_INDEFINIDO
} Tipo;

const char* tipo_para_str(Tipo t);

/* ---------- Tabela de variaveis ---------- */
typedef struct {
    char nome[MAX_NOME];
    Tipo tipo;
    int  inicializada;
    int offset;
} SimboloVar;

typedef struct {
    SimboloVar vars[MAX_VARS];
    int n;
} TabelaVariaveis;

void        ts_var_init(TabelaVariaveis *ts);
/* retorna 1 se inseriu, 0 se ja existia (redeclaracao) */
int         ts_var_insere(TabelaVariaveis *ts, const char *nome, Tipo tipo);
SimboloVar* ts_var_busca(TabelaVariaveis *ts, const char *nome);

/* Busca combinada: TS local (prioridade) e depois TS global */
SimboloVar* busca_variavel(const char *nome);
/* Marca a variavel (local ou global, na ordem de prioridade) como inicializada */
void        marca_inicializada(const char *nome);

/* ---------- Tabela de funcoes ---------- */
typedef struct {
    char nome[MAX_NOME];
    Tipo tipo;
} Parametro;

typedef struct {
    char       nome[MAX_NOME];
    Tipo       tipo_retorno;
    Parametro  params[MAX_PARAMS];
    int        n_params;
    int        prototipada;   /* 1 quando ja apareceu o prototipo  */
    int        implementada;  /* 1 quando ja apareceu a definicao  */
    int        tem_return;    /* 1 quando o corpo possui um return */
} SimboloFuncao;

typedef struct {
    SimboloFuncao funcs[MAX_FUNCS];
    int n;
} TabelaFuncoes;

void            ts_func_init(TabelaFuncoes *ts);
SimboloFuncao*  ts_func_busca(const char *nome);
/* cria (ou retorna, se ja existir) a entrada do prototipo */
SimboloFuncao*  ts_func_insere_prototipo(const char *nome, Tipo tipo_retorno);
void            ts_func_adiciona_param(SimboloFuncao *f, const char *nome, Tipo tipo);
SimboloVar* busca_variavel_info(const char *nome, int *is_local);
int         tamanho_locais_atual(void);

/* ---------- TS's ativas durante a analise ---------- */
extern TabelaVariaveis ts_global;   /* variaveis de escopo global               */
extern TabelaVariaveis ts_local;    /* variaveis da funcao sendo processada     */
extern TabelaFuncoes   ts_funcoes;  /* prototipos/implementacoes de funcoes     */
extern SimboloFuncao  *funcao_atual;/* funcao cujo corpo esta sendo analisado   */
extern int             dentro_de_funcao; /* 0 = escopo global, 1 = dentro de funcao/main */
extern int             inserindo_parametro; /* 1 = dentro da lista de parametros de uma funcao */

void imprime_tabelas(void);

#endif