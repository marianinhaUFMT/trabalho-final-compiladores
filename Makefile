compiler: tab_simb.tab.c lex.yy.c main.o tab_simb.tab.o lex.yy.o
	gcc -o comp main.o lex.yy.o tab_simb.tab.o

tab_simb.tab.c: tab_simb.y
	bison -d tab_simb.y

tab_simb.tab.o: tab_simb.tab.c 
	gcc -c tab_simb.tab.c 

lex.yy.c: tab_simb.l
	flex tab_simb.l

lex.yy.o: lex.yy.c 
	gcc -c lex.yy.c

main.o: main.c tab_simb.tab.c
	gcc -c main.c 

clean:
	rm -f *.o *.tab.c *.tab.h lex.yy.c comp