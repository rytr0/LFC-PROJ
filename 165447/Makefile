
LEX = lex
YACC = yacc -d

CC = cc

#interpreter: y.tab.o lex.yy.o logger.o functions.o interpreter.o
	#$(CC) -o $@ y.tab.o lex.yy.o logger.o functions.o interpreter.o #-ll -lm

interpreter: y.tab.o lex.yy.o functions.o interpreter.o
		$(CC) -o $@ y.tab.o lex.yy.o  functions.o interpreter.o #-ll -lm


interpreter.o: interpreter.c

lex.yy.o: lex.yy.c y.tab.h

y.tab.c y.tab.h: parser.y
	$(YACC) -v parser.y

lex.yy.c: lexer.l
	$(LEX) lexer.l

#logger.o: logger.c

functions.o: functions.c

clean:
	-rm -f *.o lex.yy.c *.tab.*  interpreter *.output

cleanlog:
	cp /dev/null log/interpreter.log
