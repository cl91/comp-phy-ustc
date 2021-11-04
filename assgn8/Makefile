all:	code report

code:	main.c count_freq.c count_freq.h
	cc -Wall main.c count_freq.c -lm
	./a.out
	gnuplot -e "set term png; set output 'sum2.png';		\
	plot 'result_sum2' with l, exp(-x**2)/sqrt(pi);		\
	set output 'sum3.png';					\
	plot 'result_sum3' with l, exp(-1.5*x**2)/sqrt(2*pi/3)"

report:	assgn8.tex
	xelatex assgn8

clean:
	rm a.out sum{2,3}.png result_sum{2,3} assgn8.{aux,log,pdf}
