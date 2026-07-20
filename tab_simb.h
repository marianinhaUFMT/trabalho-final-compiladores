#ifndef TAB_SIMB_H
#define TAB_SIMB_H

#define MAX_NOME    64
#define MAX_VARS    200
#define MAX_FUNCS   100
#define MAX_PARAMS  20

/* ---------- Tipos Primitivos ---------- */
typedef enum {
    T_INT,
    T_FLOAT,
    T_CHAR,
    T_BOOL,
    T_STRING,
    T_INDEFINIDO
} Tipo;

const char* tipo_para_str(Tipo t);

/* ---------- Estrutura de Símbolo (Variável/Parâmetro) ---------- */
typedef struct {
    char nome[MAX_NOME];
    Tipo tipo;
    int  inicializada;
    int  offset; /* Deslocamento x86 em relação ao EBP */
} SimboloVar;

/* ---------- Estrutura do Escopo Encadeado ---------- */
typedef struct Escopo {
    SimboloVar vars[MAX_VARS];
    int n;
    struct Escopo *pai; /* Ponteiro de encadeamento hierárquico (Pai/Escopo Superior) */
} Escopo;

/* ---------- Funções de Gerenciamento da Pilha de Escopos ---------- */
Escopo* criar_escopo(Escopo *pai);
void    entra_escopo(void);
void    sai_escopo(void);

/* Insere variável no escopo ATUAL */
int     ts_var_insere(Escopo *e, const char *nome, Tipo tipo);

/* Busca restrita ao escopo informado */
SimboloVar* ts_var_busca_no_escopo(Escopo *e, const char *nome);

/* Busca Combinada Encadeada: Percorre do escopo_atual subindo pelos pais até o escopo_global */
SimboloVar* busca_variavel(const char *nome);
SimboloVar* busca_variavel_info(const char *nome, int *is_local);

void    marca_inicializada(const char *nome);

/* ---------- Tabela de Funções ---------- */
typedef struct {
    char nome[MAX_NOME];
    Tipo tipo;
} Parametro;

typedef struct {
    char       nome[MAX_NOME];
    Tipo       tipo_retorno;
    Parametro  params[MAX_PARAMS];
    int        n_params;
    int        prototipada;   /* 1 quando possui protótipo  */
    int        implementada;  /* 1 quando possui definição  */
    int        tem_return;    /* 1 quando contém comando return */
} SimboloFuncao;

typedef struct {
    SimboloFuncao funcs[MAX_FUNCS];
    int n;
} TabelaFuncoes;

void            ts_func_init(TabelaFuncoes *ts);
SimboloFuncao*  ts_func_busca(const char *nome);
SimboloFuncao*  ts_func_insere_prototipo(const char *nome, Tipo tipo_retorno);
void            ts_func_adiciona_param(SimboloFuncao *f, const char *nome, Tipo tipo);

int             tamanho_locais_atual(void);

/* ---------- Variáveis Globais de Estado do Compilador ---------- */
extern Escopo        *escopo_global;     /* Escopo base (pai = NULL) */
extern Escopo        *escopo_atual;      /* Topo da pilha de escopos ativos */
extern TabelaFuncoes  ts_funcoes;        /* Tabela de protótipos/funções */
extern SimboloFuncao *funcao_atual;      /* Função sob análise */
extern int            dentro_de_funcao;  /* Controle de escopo de função */
extern int            inserindo_parametro; /* Controle de cálculo de offset x86 */

void imprime_tabelas(void);

#endif