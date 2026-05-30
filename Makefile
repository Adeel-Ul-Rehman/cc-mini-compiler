CC = g++
CFLAGS = -Wall -g
LEX = flex
YACC = bison -d

all: compiler

lexer.c: lexer.l
	$(LEX) -o lexer.c lexer.l

parser.c parser.h: parser.y
	$(YACC) -o parser.c parser.y

compiler: lexer.c parser.c ast.cpp symbol_table.cpp main.cpp
	$(CC) $(CFLAGS) -o compiler lexer.c parser.c ast.cpp symbol_table.cpp main.cpp

clean:
	rm -f lexer.c parser.c parser.h compiler *.o

run: compiler
	./compiler test.c