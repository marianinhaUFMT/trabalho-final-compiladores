float total;
float media;
bool impar;
string empresa;
float soma(float a, float b);
float soma(float a, float b) {
    return (a + b);
}
int main() {
    string nome;
    float x;
    float y;
    empresa = "UFMT";
    write ("Escreva seu nome: ");
    read (nome);
    read (x);
    read (y);
    total = soma(x,y);
    media = total/2;
    impar = False;
    while (total > 0) {
        if (impar == False) {
            impar = True;
        } else {
            impar = False;
        }
        total = total-1;
    }
    write (empresa);
    write (nome);
    write ("Sua media eh: ");
    write (media);
    write ("Seu numero eh: ");
    if (impar == True) {
        write ("impar");
    } else {
        write ("par");
    }
    return 0;
}