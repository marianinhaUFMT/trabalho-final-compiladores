int main() {
    int x;

    // ERRO LÉXICO: Caractere inválido ($)
    x = 10 $ 2;

    // ERRO SINTÁTICO: Ponto e vírgula esquecido / estrutura malformada
    // x = 10 + 
    
    // ERRO SINTÁTICO: Bloco não fechado
    // if (x > 0) {
    //    write(x);

    return 0;
}