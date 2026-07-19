%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_simb.h"
#include "parser.tab.h"

void yyerror(const char *);
int linha_atual = 1;
%}

%option noyywrap

%%
"//".*    { /* ignora comentário */ }

"int"                   { yylval.c.tipo = T_INT;    return INT; }
"float"                 { yylval.c.tipo = T_FLOAT;  return FLOAT; }
"char"                  { yylval.c.tipo = T_CHAR;   return CHAR; }
"bool"                  { yylval.c.tipo = T_BOOL;   return BOOL; }
"string"                { yylval.c.tipo = T_STRING; return STRING; }

"if"                    { return IF; }
"else"                  { return ELSE; }
"while"                 { return WHILE; }
"read"                  { return READ; }
"write"                 { return WRITE; }
"return"                { return RETURN; }
"True"                  { return TRUE; }
"False"                 { return FALSE; }
"main"                  { return MAIN; }

"=="                    { return EQ; }
"!="                    { return NE; }
"!"                     { return NOT; }
"<="                    { return LE; }
"<"                     { return LT; }
">="                    { return GE; }
">"                     { return GT; }


[a-zA-Z_][a-zA-Z0-9_]* { 	
                printf("[LEX] Variavel '%s'\n",yytext);
                strcpy(yylval.c.str, yytext); 
                return VARIABLE; 
            } 

[0-9]+\.[0-9]+ {
                strcpy(yylval.c.str, yytext);
                printf("[LEX] Float: %s\n", yytext);
                return FLOAT_CONST;

            }

[0-9]+      { 	
                strcpy(yylval.c.str, yytext);
		        printf("[LEX] Inteiro: %s\n", yytext);
                return INTEGER; 
            }

'[^']'      { 
                yylval.c.str[0] = yytext[1]; // pega o caractere entre aspas simples
                yylval.c.str[1] = '\0'; // adiciona o terminador nulo
                printf("[LEX] Caractere: %c\n",yylval.c.str[0]);
                return CHAR_CONST; 
            }
            
\"[^\"]*\"  { 
                // tira as aspas para armazenar a string literal
                strncpy(yylval.c.str, yytext+1, strlen(yytext)-2);
                yylval.c.str[strlen(yytext)-2] = '\0'; // adiciona o terminador nulo
                printf("[LEX] String: %s\n",yytext);
                return STRING_CONST; 
            }
                
[=+\-/*(){};,]   { 	
                //strcpy(yylval.c.lex, yytext);
                //yylval.c.val = *yytext;
                printf("[LEX] Operador: %c\n", yytext[0]);
                return *yytext; 
            }

"\n"        { linha_atual++; }


[ \t]+    	;

.           { 
                char msg[256];
                sprintf(msg, "[LEX ERROR] Caracter invalido '%s' na linha %d", yytext, linha_atual);
                yyerror(msg); 
            }

%%