all:	code report

code:	main.c
	cc -Wall main.c -lm
	./a.out
	echo `cat result_1`
	echo `cat result_2`

report:	assgn7.tex
	xelatex assgn7

clean:
	rm a.out result_{1,2} assgn7.{aux,log,pdf}
