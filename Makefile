.PHONY=clean all

all : make_compiler

make_compiler : yacc lex compile
 
lex : scanner.l
	flex scanner.l 

yacc : grammar.y 
	yacc -d grammar.y

compile : lex.yy.c y.tab.c node.c translator.c utils.c
	gcc -Wall -fsanitize=address lex.yy.c y.tab.c node.c translator.c utils.c -o compiler

clean :	compiler lex.yy.c y.tab.c y.tab.h
	rm -rf compiler lex.yy.c y.tab.c y.tab.h

