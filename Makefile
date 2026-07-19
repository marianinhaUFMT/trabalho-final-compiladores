
compiler: parser.tab.c lex.yy.c tab_simb.c main.c tab_simb.h
	$(CC) $(CFLAGS) -o comp parser.tab.c lex.yy.c tab_simb.c main.c

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

tab_simb.tab.o: tab_simb.tab.c 
	gcc -c tab_simb.tab.c

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

lex.yy.o: lex.yy.c 
	gcc -c lex.yy.c

main.o: main.c tab_simb.tab.c
	gcc -c main.c 

clean:
	rm -f *.o *.tab.c *.tab.h lex.yy.c comp
