all:	code report
code:	main.c count_freq.c
	cc -Wall main.c count_freq.c
	./a.out 0 freq_0 400 10000000 5 temp_0
	./a.out 1 freq_1 400 10000000 5 temp_1
	gnuplot -e "set term png; set output 'freq_0.png';	\
		plot 'freq_0' with l"
	gnuplot -e "set term png; set output 'freq_1.png';	\
		plot 'freq_1' with l"
report:	assgn17.tex
	xelatex assgn17
	xelatex assgn17

clean:
	-rm assgn17.{pdf,aux,log} a.out freq_{0,1}{,.png} temp_{0,1}
