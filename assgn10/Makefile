all:	main.c count_freq.c count_freq.h plot.py
	cc -Wall main.c count_freq.c -lm
	./a.out
	./plot.py
	xelatex assgn10
	xelatex assgn10

clean:
	rm res{,_exp} {res,exp,freq}.png freq 	\
		fit.log assgn10.{aux,log,pdf} exp a.out
