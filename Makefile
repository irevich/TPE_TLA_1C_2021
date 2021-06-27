.PHONY=clean all test clean_test

all : make_compiler

make_compiler : yacc lex compile
 
lex : scanner.l
	flex scanner.l 

yacc : grammar.y 
	yacc -d grammar.y

compile : lex.yy.c y.tab.c node.c translator.c utils.c
	gcc -Wall -fsanitize=address lex.yy.c y.tab.c node.c translator.c utils.c -o compiler

clean :
	rm -rf compiler lex.yy.c y.tab.c y.tab.h

test : tests/test1.geome tests/test2.geome tests/test3.geome tests/test4.geome tests/test5.geome compiler
	./compiler < tests/test1.geome > generated_tests/test1.c
	./compiler < tests/test2.geome > generated_tests/test2.c
	./compiler < tests/test3.geome > generated_tests/test3.c
	./compiler < tests/test4.geome > generated_tests/test4.c
	./compiler < tests/test5.geome > generated_tests/test5.c
	gcc generated_tests/test1.c -o executable_tests/test1 -lm
	gcc generated_tests/test2.c -o executable_tests/test2 -lm
	gcc generated_tests/test3.c -o executable_tests/test3 -lm
	gcc generated_tests/test4.c -o executable_tests/test4 -lm
	gcc generated_tests/test5.c -o executable_tests/test5 -lm

clean_test :
	rm -rf generated_tests/*
	rm -rf executable_tests/*



