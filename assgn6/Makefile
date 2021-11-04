all:	code

code:	main.c count_freq.c count_freq.h
	cc -Wall main.c count_freq.c
	./a.out
	gnuplot -e "set term png; set output 'direct.png';		\
		plot 'freq_data' with lines, 'freq_direct' with lines;	\
		set output 'accrej.png';				\
		plot 'freq_data' with lines, 'freq_accrej' with lines;"
	xelatex assgn6

clean:
	rm a.out freq_{data,direct,accrej} {direct,accrej}.png 		\
		assgn6.{pdf,aux,log}
