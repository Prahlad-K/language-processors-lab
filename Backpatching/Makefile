obj:	example.l example.y 	
	flex example.l
	bison -d example.y
	gcc example.tab.c lex.yy.c -lfl -o example
