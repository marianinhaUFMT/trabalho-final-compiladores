#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yyparse(void);
extern FILE *yyin;

int main(int argc, char const *argv[]) {
    if (argc !=2) {
        printf("Informe um arquivo de entrada como paramatro de execucao:\n $ %s <arquivo>\n", argv[0]);
        return EXIT_FAILURE;
    }
	yyin = fopen(argv[1], "r");
    int ret = yyparse(); // chamada do analisador sintatico
    return ret;    
}