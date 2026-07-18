%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    void yyerror(char *); 
    int yylex(void); 

    // Definicao da estrutura para a TS
    struct tab_simb {
        int val;
        char lex[64];
    };
    typedef struct tab_simb ts;

    ts tabela[100];
    int cont_tab = 0;
%}

%union {
    struct code_t {
		char str[256];
		int val; 
	} c;
}

%type <c> program statement expr
%token <c> INTEGER 
%token <c> VARIABLE 
%left '+' '-' 
%left '*' '/' 

%% 

program:  
        program statement 
        | %empty { }
        ; 
statement: 
        expr                    { printf("[SYN] Valor exp: %d\n", $1.val); } 
        | VARIABLE '=' expr     { 
                                    strcpy( tabela[cont_tab].lex,  $1.str);
                                    tabela[cont_tab].val = $3.val;
                                    cont_tab++;
                                } 
        ; 
expr: 
        INTEGER                 { $$.val = $1.val; }
        | VARIABLE              { 
                                    /*TODO: implemente uma funcao que busca na TS a variavel e seu valor*/
                                    int indice_encontrado = -1;

                                    for (int i=0; i < cont_tab; i++) {
                                        if (strcmp(tabela[i].lex, $1.str) == 0) {
                                            indice_encontrado = i;
                                            break;
                                        }
                                    }

                                    if (indice_encontrado != -1) {
                                        $$.val = tabela[indice_encontrado].val;
                                    } else {
                                        char msg_erro[128];
                                        sprintf(msg_erro, "[SEMANTIC ERROR] Variavel '%s' de multiplos caracteres nao foi inicializada.", $1.str);
                                        yyerror(msg_erro);
                                        exit(EXIT_FAILURE);
                                    }

                                } 
        | expr '+' expr         { $$.val = $1.val + $3.val; } 
        | expr '-' expr         { $$.val = $1.val - $3.val; } 
        | expr '*' expr         { $$.val = $1.val * $3.val; } 
        | expr '/' expr         { $$.val = $1.val / $3.val; } 
        | '(' expr ')'          { $$.val = $2.val; } 
        ; 
%% 

void yyerror(char *s) { 
    printf("%s\n", s); 
    //return 0; 
} 

