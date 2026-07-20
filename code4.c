int multiplicar(int a, int b);

// ERRO 1: Função implementada sem protótipo prévio
int subtrair(int a, int b) { return a - b; }

// ERRO 2: Redefinição/implementação com assinatura diferente do protótipo
int multiplicar(int a, float b) {
    return 0;
}

int main() {
    int x;
    string nome;

    // ERRO 3: Uso de variável não declarada
    total = 10;

    // ERRO 4: Redeclaração de variável no mesmo escopo
    int x;

    // ERRO 5: Atribuição com tipo incompatível (string para int)
    x = "Texto";

    // ERRO 6: Chamada de função com quantidade incorreta de parâmetros
    x = multiplicar(5);

    // ERRO 7: Chamada de função com tipos de argumentos incompatíveis
    x = multiplicar("A", "B");

    return 0;
}

// ERRO 8: Função sem comando 'return'
int sem_retorno() {
    int a;
    a = 10;
}