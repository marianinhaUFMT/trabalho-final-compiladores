# Compilador para Linguagem C Simplificada

Trabalho Final da disciplina **Laboratório de Compiladores** do curso de **Bacharelado em Ciência da Computação** da **Universidade Federal de Mato Grosso (UFMT)**.

## 📖 Descrição

Este projeto consiste na implementação de um compilador para uma versão simplificada da linguagem C utilizando **Flex** e **Bison**. O compilador realiza as etapas de análise léxica, sintática e semântica, gerando como saída código em **Assembly x86** para programas escritos na linguagem especificada.

Além da geração de código, o compilador identifica e informa erros léxicos, sintáticos e semânticos, indicando a linha e a posição onde ocorreram.

## ✨ Funcionalidades

- Análise léxica com Flex
- Análise sintática Bottom-Up com Bison
- Análise semântica
- Gerenciamento de tabelas de símbolos
  - Escopo global
  - Escopos locais
  - Tabela de funções
- Geração de código Assembly x86
- Detecção de erros:
  - Léxicos
  - Sintáticos
  - Semânticos
- Impressão das tabelas de símbolos ao final da compilação

## 📝 Recursos da Linguagem

### Tipos suportados

- `int`
- `float`
- `char`
- `bool`
- `string`

### Estruturas suportadas

- Declaração de variáveis
- Escopo por blocos (`{}`)
- Atribuições
- Expressões aritméticas
- Expressões relacionais
- Expressões lógicas
- Comando `if`
- Comando `if/else`
- Laço `while`
- Entrada de dados (`read`)
- Saída de dados (`write`)
- Declaração e implementação de funções
- Protótipos de funções
- Chamada de funções
- Retorno com `return`

## 📂 Estrutura do Projeto

```
.
├── lexer.l             # Analisador léxico (Flex)
├── parser.y            # Analisador sintático (Bison)
├── symbols.*           # Tabelas de símbolos
├── semantic.*          # Análise semântica
├── codegen.*           # Geração de código Assembly
├── examples/           # Programas de exemplo
├── output/             # Arquivos Assembly gerados
├── Makefile
└── README.md
```

> A estrutura pode variar conforme a implementação do projeto.

## ⚙️ Tecnologias Utilizadas

- C
- Flex
- Bison
- GCC
- Make

## 🚀 Como Compilar

```bash
make
```

ou

```bash
flex lexer.l
bison -d parser.y
gcc lex.yy.c parser.tab.c -o compilador -lfl
```

## ▶️ Como Executar

```bash
./compilador exemplo.c
```

O compilador irá gerar o código Assembly correspondente ao programa de entrada.

## 📌 Exemplo

### Entrada

```c
int soma(int a, int b);

int soma(int a, int b){
    return a + b;
}

int main(){
    int x;
    x = soma(5,3);
    return 0;
}
```

### Saída

O compilador gera o código equivalente em Assembly x86, incluindo:

- prólogo e epílogo das funções;
- passagem de parâmetros pela pilha;
- instruções `call`;
- retorno pelo registrador `eax`.

## 📚 Conceitos Aplicados

- Compiladores
- Autômatos Finitos
- Expressões Regulares
- Gramáticas Livres de Contexto
- Parsing Bottom-Up (LALR)
- Análise Léxica
- Análise Sintática
- Análise Semântica
- Tabelas de Símbolos
- Escopos
- Geração de Código Intermediário
- Geração de Código Assembly

## 👨‍🏫 Disciplina

**Laboratório de Compiladores**

Universidade Federal de Mato Grosso (UFMT)

Professor: **Dr. Ivairton M. Santos**

## 👥 Autores

- Mariana Sanchez Pedroni
- Anna Bheatryz Martins dos Santos

## 📄 Licença

Este projeto foi desenvolvido exclusivamente para fins acadêmicos.