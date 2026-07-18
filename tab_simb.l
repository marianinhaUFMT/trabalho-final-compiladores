%{ 
    #include <stdio.h>
    #include <stdlib.h> 
    #include <string.h>
    void yyerror(char *); 
    #include "tab_simb.tab.h"
    int linha_atual = 1; // detalha erros 
%} 

%% 
// palavras reservadas
"int"       { return INT; }
"float"     { return FLOAT; }
"char"      { return CHAR; }
"bool"      { return BOOL; }
"string"    { return STRING; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"read"      { return READ; }
"write"     { return WRITE; }
"return"    { return RETURN; }

// operadores logicos
"=="        { return EQ; }
"!="        { return NE; }
"<="        { return LE; }
">="        { return GE; }

[a-zA-Z_][a-zA-Z0-9_]* { 	
                printf("[LEX] Variavel '%s'\n",yytext);
                strcpy(yylval.c.str, yytext); 
                return VARIABLE; 
            } 

[0-9]+      { 	
                strcpy(yylval.c.str, yytext);
                yylval.c.val = atoi(yytext);
		        printf("[LEX] Inteiro: %d\n",yylval.c.val);
                return INTEGER; 
            }
[0-9]+\.[0-9]+ {
                strcpy(yylval.c.str, yytext);
                yylval.c.val_f = atof(yytext);
                printf("[LEX] Float: %f\n",yylval.c.val_f);
                return FLOAT;

            }
            
\"[^\"]*\"  { 
                // tira as aspas para armazenar a string literal
                strcpy(yylval.c.str, yytext+1, strlen(yytext)-2);
                yylval.c.str[strlen(yytext)-2] = '\0'; // adiciona o terminador nulo
                printf("[LEX] String: %s\n",yytext);
                return STRING; 
            }
                
[=+-/*()]   { 	
                //strcpy(yylval.c.lex, yytext);
                //yylval.c.val = *yytext;
                printf("[LEX] Operador: %c\n", yytext[0]);
                return *yytext; 
            } 

[ \t\n]    	; 

.             	yyerror("[LEX] [ERROR] Caracter invalido"); 

%%
 
int yywrap(void) { 
    return 1; 
} 
