compiler: parser.tab.o lex.yy.o tab_simb.o codeGeneration.o main.o
	$(CC) $(CFLAGS) -o comp parser.tab.o lex.yy.o tab_simb.o codeGeneration.o main.o -lfl

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) -c parser.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

tab_simb.o: tab_simb.c tab_simb.h
	$(CC) $(CFLAGS) -c tab_simb.c

codeGeneration.o: codeGeneration.c codeGeneration.h
	$(CC) $(CFLAGS) -c codeGeneration.c

main.o: main.c tab_simb.h codeGeneration.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o *.tab.c *.tab.h lex.yy.c comp saida.asm parser.output