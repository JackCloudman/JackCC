Gram=y.tab.c y.tab.h

all: $(Gram) lex.yy.c complejo_cal.c
	@gcc -o calc y.tab.c lex.yy.c code.c complejo_cal.c Symbol.c init.c -lm
	@echo Compiled

complejo_cal.c:
	@echo "Si esta"

$(Gram): complejo_cal.y
	@yacc -d complejo_cal.y

lex.yy.c: complejo_cal.l
	@flex complejo_cal.l

clean:
	@rm -f *.out lex.yy.c *.tab.* calc
	@echo Clean
