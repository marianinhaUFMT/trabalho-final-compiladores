#ifndef CODE_H
#define CODE_H

#include <stdio.h>
#include <stdlib.h>

#define STRING_SIZE 256
#define CODE_BUF 8192

/* ---- expressoes / valores ---- */
void makeCodeLoadConst(char *dest, char *numlit);
void makeCodeLoadVarGlobal(char *dest, char *varname);
void makeCodeLoadVarLocal(char *dest, char *varname, int offset);
void makeCodeLoadString(char *dest, char *label);
void makeCodeWriteStringLiteral(char *dest, char *str_literal, int label_id);

void makeCodeAdd(char *dest, char *value);
void makeCodeSub(char *dest, char *value);
void makeCodeMul(char *dest, char *value2);
void makeCodeDiv(char *dest, char *value2);
void makeCodeMod(char *dest, char *value2);

/* op: "==", "!=", "<", "<=", ">", ">=" */
void makeCodeRel(char *dest, char *value, const char *op);
void makeCodeAnd(char *dest, char *value);
void makeCodeOr(char *dest, char *value);
void makeCodeNot(char *dest);

/* ---- atribuicao ---- */
void makeCodeAssignGlobal(char *dest, char *expr_code, char *varname);
void makeCodeAssignLocal(char *dest, char *expr_code, char *varname, int offset);

/* ---- controle de fluxo ---- */
int  novoLabel(void); /* gera um numero de rotulo unico */
void makeCodeIf(char *dest, char *cond_code, char *then_code, char *else_code, int has_else, int l1, int l2);
void makeCodeWhile(char *dest, char *cond_code, char *body_code, int l1, int l2);

/* ---- funcoes ---- */
void makeCodeFuncProlog(char *dest, char *funcname, int tam_variaveis_locais);
void makeCodeFuncEpilog(char *dest);
void makeCodeReturn(char *dest, char *expr_code);
void makeCodeCallBegin(char *dest); /* zera buffer para acumular pushes de argumentos, em ordem inversa */
void makeCodeCallPushArg(char *dest, char *arg_code);
void makeCodeCallEnd(char *dest, char *funcname, int n_args);

/* ---- E/S ---- */
void makeCodeWrite(char *dest, char *value_code, int tipo_str); /* tipo_str=1 se for label de string */
void makeCodeRead(char *dest, char *varname, int is_local, int offset);

/* ---- declaracao de variaveis globais (secao .data / .bss) ---- */
void makeCodeDeclGlobal(char *dest, char *varname);

#endif