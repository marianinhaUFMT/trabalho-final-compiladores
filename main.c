#include <stdio.h>
#include <stdlib.h>
#include "tab_simb.h"

int yyparse(void);
extern FILE *yyin;
extern FILE *output_file;
extern int erros_encontrados;

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Informe um arquivo de entrada como parametro de execucao:\n $ %s <arquivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("[ERROR] Nao foi possivel abrir o arquivo '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    output_file = fopen("saida.asm", "w");
    if (!output_file) {
        printf("[ERROR] Nao foi possivel criar o arquivo de saida\n");
        return EXIT_FAILURE;
    }

    ts_var_init(&ts_global);
    ts_var_init(&ts_local);
    ts_func_init(&ts_funcoes);

    int ret = yyparse();

    fclose(yyin);
    fclose(output_file);

    imprime_tabelas();

    if (ret == 0 && erros_encontrados == 0) {
        printf("\n[COMPILER] Analise concluida sem erros. Codigo assembly gerado em 'saida.asm'.\n");
    } else {
        printf("\n[COMPILER] Analise concluida com %d erro(s) semantico(s).\n", erros_encontrados);
    }

    return (ret == 0 && erros_encontrados == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}