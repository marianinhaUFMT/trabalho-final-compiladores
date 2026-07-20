# Compilador para Linguagem C Simplificada

Trabalho Final da disciplina **Laboratório de Compiladores** do curso de **Bacharelado em Ciência da Computação** da **Universidade Federal de Mato Grosso (UFMT)**.

## 📖 Descrição

Este projeto consiste na implementação de um compilador para uma versão simplificada da linguagem C utilizando as ferramentas **Flex** e **Bison**. O compilador realiza todas as etapas do processo de compilação (análise léxica, sintática, semântica e geração de código), produzindo como saída o código de montagem em **Assembly x86 (32-bits, sintaxe NASM)** correspondente.

Caso o código-fonte possua erros (léxicos, sintáticos ou semânticos), o compilador detalha a falha informando o tipo de erro, descrição e a linha exata no terminal.

## ✨ Funcionalidades

- **Análise Léxica (Flex):** Reconhecimento de tokens, palavras reservadas, literais, operadores e controle de linha.
- **Análise Sintática (Bison):** Análise bottom-up LALR(1) para construção da árvore sintática.
- **Análise Semântica:**
  - Checagem de compatibilidade de tipos.
  - Verificação de declaração/redeclaração de variáveis no mesmo escopo.
  - Validação de protótipos de funções, parâmetros e retornos obrigatórios.
- **Tabelas de Símbolos:**
  - Tabela de Variáveis Globais.
  - Tabela de Variáveis Locais com escopo encadeado (com precedência do escopo local).
  - Tabela de Funções separada com validação de assinaturas.
- **Geração de Código Assembly x86:**
  - Manipulação da pilha para alocação de variáveis locais via offsets em `$ebp`.
  - Passagem de argumentos de funções pela pilha em ordem inversa.
  - Prólogo, epílogo e retorno armazenado no registrador `$eax`.
  - Controle de fluxo (`if`, `if/else`, `while`).
  - Suporte às instruções de Entrada e Saída (`read` e `write`).
- **Relatório Final:** Impressão gráfica das tabelas de símbolos após o término da compilação.

## 📝 Recursos da Linguagem

### Tipos suportados
- `int`
- `float`
- `char`
- `bool`
- `string`

### Estruturas suportadas
- Declaração de variáveis (`<tipo> <identificador>;`)
- Atribuições (`<identificador> = <expressão>;`)
- Operadores aritméticos (`+`, `-`, `*`, `/`)
- Operadores lógicos e relacionais (`==`, `!=`, `!`, `<`, `<=`, `>`, `>=`)
- Comandos condicionais (`if` e `if/else`)
- Laço de repetição (`while`)
- Entrada e Saída de dados (`read` e `write`)
- Protótipo de funções (`<tipo> <identificador>(<params>);`)
- Chamada e implementação de funções com o comando `return`

## 📂 Estrutura do Projeto


```

.
├── lexer.l             # Analisador Léxico (Flex)
├── parser.y            # Analisador Sintático / Semântico (Bison)
├── tab_simb.h          # Definições das estruturas das Tabelas de Símbolos
├── tab_simb.c          # Implementação da TS (globais, locais e funções)
├── codeGeneration.h    # Protótipos para emissão de código Assembly x86
├── codeGeneration.c    # Gerador de instruções x86 (pilha, rótulos, prólogo/epílogo)
├── main.c              # Ponto de entrada do compilador
├── Makefile            # Automação de compilação do compilador
└── README.md           # Documentação do projeto

```

## ⚙️ Requisitos e Tecnologias

- **GCC** (suporte a 32-bits via `-m32` e bibliotecas `gcc-multilib`)
- **Flex**
- **Bison**
- **NASM** (Netwide Assembler)
- **Make**

## 🚀 Como Compilar o Compilador

Para compilar o código do próprio compilador (`comp`), execute:

```bash
make

```

Para limpar os arquivos objetos e executáveis gerados:

```bash
make clean

```

## ▶️ Como Executar e Gerar o Código Assembly

Execute o compilador informando o arquivo com o código em C simplificado:

```bash
./comp arquivo_de_teste.c

```

Se o código estiver correto, o compilador exibirá as Tabelas de Símbolos no terminal e gerará o arquivo **`saida.asm`** com o código Assembly x86.


## 👨‍🏫 Disciplina

**Laboratório de Compiladores**

Universidade Federal de Mato Grosso (UFMT)

Instituto de Ciências Exatas e da Terra (ICET)

Professor: **Dr. Ivairton M. Santos**

## 👥 Autores

* **Anna Bheatryz Martins dos Santos**
* **Mariana Sanchez Pedroni**

## 📄 Licença

Este projeto foi desenvolvido exclusivamente para fins acadêmicos.
