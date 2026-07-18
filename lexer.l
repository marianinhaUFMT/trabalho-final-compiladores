%{
    #include <stdio.h>
    #include <stdlib.h>
    // depois descomentar o include abaixo do sintatico gerado pelo BISON
    // #include "parser.tab.h"
%}

// SECAO 1
// DEFINICOES REGULARES E DECLARACOES C
digit      [0-9]
letter     [a-zA-Z]
id        {letter}({letter}|{digit})*
ws        [ \t\n\r]

// SECAO 2
// DEFINICOES DE REGRAS DE TRADUCAO E RETORNO SINTATICO

{ws}+        { /* ignora espacos em branco */ }
// palavras reservadas
"int"        { return INT; }
"float"      { return FLOAT; }
"char"       { return CHAR; }
"bool"       { return BOOL; }
"string"     { return STRING; }
"if"         { return IF; }
"else"       { return ELSE; }
"while"      { return WHILE; }
"read"       { return READ; }
"write"      { return WRITE; }
"return"     { return RETURN; }
"true"       { return TRUE; }
"false"      { return FALSE; }

// identificadores
{id}         { return ID; }

{digit}+            { return NUM_INT; }
{digit}+\.{digit}+  { return NUM_FLOAT; }
\"[^\"]*\"          { return STRING_LITERAL; }

// operadores logicos e relacionais
"=="         { return EQ; }
"!="         { return NEQ; }
"<="         { return LE; }
">="         { return GE; }
"<"          { return LT; }
">"          { return GT; }

// operadores aritmeticos
"="         { return '='; }
"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }

// delimitadores
"{"         { return '{'; }
"}"         { return '}'; }
"("         { return '('; }
")"         { return ')'; }
";"         { return ';'; }
","         { return ','; }

.           {printf("[ERRO LEXICO]: caractere invalido: '%s'\n", yytext); }

%%
// SECAO 3
// PROCEDIMENTOS DO USUARIO (FUNCAO MAIN)
int yywrap(void){
    return 1;
}