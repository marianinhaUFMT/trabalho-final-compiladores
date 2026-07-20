// Protótipos de funções
int soma(int a, int b);
bool eh_par(int n);

// Variáveis globais
int contador;
string titulo;
bool ativo;

int soma(int a, int b) {
    return a + b;
}

bool eh_par(int n) {
    if (n == 0) {
        return True;
    } else {
        return False;
    }
}

int main() {
    int x;
    int y;
    int resultado;
    char letra;

    titulo = "Teste Completo";
    contador = 5;
    letra = 'A';
    ativo = True;

    write(titulo);
    write(letra);

    write("Digite o valor de x:");
    read(x);
    write("Digite o valor de y:");
    read(y);

    resultado = soma(x, y);
    write("Resultado da soma:");
    write(resultado);

    while (contador > 0) {
        write(contador);
        if (eh_par(contador) == True) {
            write("Eh par");
        } else {
            write("Eh impar");
        }
        contador = contador - 1;
    }

    return 0;
}