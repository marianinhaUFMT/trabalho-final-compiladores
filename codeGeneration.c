#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGeneration.h"

static int label_count = 0;

int novoLabel(void) {
    return label_count++;
}

/* ================= Valores / expressoes ================= */

void makeCodeLoadConst(char *dest, char *numlit) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "mov eax, %s\n", numlit);
}

void makeCodeLoadVarGlobal(char *dest, char *varname) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "mov eax, [%s]\n", varname);
}

void makeCodeLoadVarLocal(char *dest, char *varname, int offset) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "mov eax, [ebp%+d] ; %s\n", offset, varname);
}

void makeCodeLoadString(char *dest, char *label) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "mov eax, %s\n", label);
}

void makeCodeAdd(char *dest, char *value) {
    sprintf(dest + strlen(dest), "push eax\n");
    sprintf(dest + strlen(dest), "%s", value);
    sprintf(dest + strlen(dest), "mov ecx, eax\npop eax\nadd eax, ecx\n");
}

void makeCodeWriteStringLiteral(char *dest, char *str_literal, int label_id) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "jmp Lstr_end_%d\n", label_id);
    sprintf(dest + strlen(dest), "Lstr_%d: db '%s', 0\n", label_id, str_literal);
    sprintf(dest + strlen(dest), "Lstr_end_%d:\n", label_id);
    sprintf(dest + strlen(dest), "push Lstr_%d\n", label_id);
    sprintf(dest + strlen(dest), "call print_str\n");
    sprintf(dest + strlen(dest), "add esp, 4\n");
}

void makeCodeSub(char *dest, char *value) {
    sprintf(dest + strlen(dest), "push eax\n");
    sprintf(dest + strlen(dest), "%s", value);
    sprintf(dest + strlen(dest), "mov ecx, eax\npop eax\nsub eax, ecx\n");
}

void makeCodeMul(char *dest, char *value2) {
    sprintf(dest + strlen(dest), "push eax\n");
    sprintf(dest + strlen(dest), "%s", value2);
    sprintf(dest + strlen(dest), "mov ecx, eax\npop eax\nimul eax, ecx\n");
}

void makeCodeDiv(char *dest, char *value2) {
    sprintf(dest + strlen(dest), "push eax\n");
    sprintf(dest + strlen(dest), "%s", value2);
    sprintf(dest + strlen(dest), "mov ecx, eax\npop eax\nxor edx, edx\nidiv ecx\n");
}

void makeCodeMod(char *dest, char *value2) {
    sprintf(dest + strlen(dest), "push eax\n");
    sprintf(dest + strlen(dest), "%s", value2);
    sprintf(dest + strlen(dest), "mov ecx, eax\npop eax\nxor edx, edx\nidiv ecx\nmov eax, edx\n");
}

void makeCodeRel(char *dest, char *value, const char *op) {
    sprintf(dest + strlen(dest), "push eax\n");
    sprintf(dest + strlen(dest), "%s", value);
    sprintf(dest + strlen(dest), "mov ecx, eax\npop ebx\ncmp eax, ecx\n");

    const char *instr = "sete";
    if      (strcmp(op, "==") == 0) instr = "sete";
    else if (strcmp(op, "!=") == 0) instr = "setne";
    else if (strcmp(op, "<")  == 0) instr = "setl";
    else if (strcmp(op, "<=") == 0) instr = "setle";
    else if (strcmp(op, ">")  == 0) instr = "setg";
    else if (strcmp(op, ">=") == 0) instr = "setge";
    sprintf(dest + strlen(dest), "%s al\nmovzx eax, al\n", instr);
}

void makeCodeAnd(char *dest, char *value) {
    sprintf(dest + strlen(dest), "push eax\n");
    sprintf(dest + strlen(dest), "%s", value);
    sprintf(dest + strlen(dest), "mov ecx, eax\npop eax\nand eax, ecx\n");
}

void makeCodeOr(char *dest, char *value) {
    sprintf(dest + strlen(dest), "push eax\n");
    sprintf(dest + strlen(dest), "%s", value);
    sprintf(dest + strlen(dest), "mov ecx, eax\npop eax\nor eax, ecx\n");
}

void makeCodeNot(char *dest) {
    sprintf(dest + strlen(dest), "pop eax\nxor eax, 1\npush eax\n");
}

/* ================= Atribuicao ================= */

void makeCodeAssignGlobal(char *dest, char *expr_code, char *varname) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "%s", expr_code);
    sprintf(dest + strlen(dest), "mov [%s], eax\n", varname);
}

void makeCodeAssignLocal(char *dest, char *expr_code, char *varname, int offset) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "%s", expr_code);
    sprintf(dest + strlen(dest), "mov [ebp%+d], eax ; %s\n", offset, varname);
}

/* ================= Controle de fluxo ================= */

void makeCodeIf(char *dest, char *cond_code, char *then_code, char *else_code, int has_else, int l1, int l2) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "%s", cond_code);
    sprintf(dest + strlen(dest), "cmp eax, 0\n");
    if (has_else) {
        sprintf(dest + strlen(dest), "je L%d_else\n", l1);
        sprintf(dest + strlen(dest), "%s", then_code);
        sprintf(dest + strlen(dest), "jmp L%d_end\n", l2);
        sprintf(dest + strlen(dest), "L%d_else:\n", l1);
        sprintf(dest + strlen(dest), "%s", else_code);
        sprintf(dest + strlen(dest), "L%d_end:\n", l2);
    } else {
        sprintf(dest + strlen(dest), "je L%d_end\n", l2);
        sprintf(dest + strlen(dest), "%s", then_code);
        sprintf(dest + strlen(dest), "L%d_end:\n", l2);
    }
}

void makeCodeWhile(char *dest, char *cond_code, char *body_code, int l1, int l2) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "L%d_start:\n", l1);
    sprintf(dest + strlen(dest), "%s", cond_code);
    sprintf(dest + strlen(dest), "cmp eax, 0\nje L%d_end\n", l2);
    sprintf(dest + strlen(dest), "%s", body_code);
    sprintf(dest + strlen(dest), "jmp L%d_start\n", l1);
    sprintf(dest + strlen(dest), "L%d_end:\n", l2);
}

/* ================= Funcoes ================= */

void makeCodeFuncProlog(char *dest, char *funcname, int tam_variaveis_locais) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "%s:\n", funcname);
    sprintf(dest + strlen(dest), "push ebp\nmov ebp, esp\n");
    if (tam_variaveis_locais > 0) {
        sprintf(dest + strlen(dest), "sub esp, %d\n", tam_variaveis_locais);
    }
}

void makeCodeFuncEpilog(char *dest) {
    sprintf(dest + strlen(dest), "mov esp, ebp\npop ebp\nret\n");
}

void makeCodeReturn(char *dest, char *expr_code) {
    sprintf(dest + strlen(dest), "%s", expr_code);
    sprintf(dest + strlen(dest), "mov esp, ebp\npop ebp\nret\n");
}

void makeCodeCallBegin(char *dest) {
    dest[0] = '\0';
}

void makeCodeCallPushArg(char *dest, char *arg_code) {
    /* arg_code deixa o valor no topo da pilha (push ...); convertido para "push" direto do valor */
    sprintf(dest + strlen(dest), "%s", arg_code);
    sprintf(dest + strlen(dest), "push eax\n");
}

void makeCodeCallEnd(char *dest, char *funcname, int n_args) {
    sprintf(dest + strlen(dest), "call %s\n", funcname);
    if (n_args > 0) {
        sprintf(dest + strlen(dest), "add esp, %d\n", n_args * 4);
    }
}

/* ================= E/S ================= */

void makeCodeWrite(char *dest, char *value_code, int tipo_str) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "%s", value_code);
    sprintf(dest + strlen(dest), "push eax\n");
    if (tipo_str) {
        sprintf(dest + strlen(dest), "call print_str\n");
    } else {
        sprintf(dest + strlen(dest), "call print_int\n");
    }
    sprintf(dest + strlen(dest), "add esp, 4\n");
}

void makeCodeRead(char *dest, char *varname, int is_local, int offset) {
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "call read_int\n");
    if (is_local) {
        sprintf(dest + strlen(dest), "mov [ebp%+d], eax ; %s\n", offset, varname);
    } else {
        sprintf(dest + strlen(dest), "mov [%s], eax\n", varname);
    }
}

/* ================= Declaracao global ================= */

void makeCodeDeclGlobal(char *dest, char *varname) {
    sprintf(dest + strlen(dest), "%s: dd 0\n", varname);
}