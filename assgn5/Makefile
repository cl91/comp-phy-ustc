all:	code report

code:	main.c
	cc -Wall main.c -lm
	./a.out
	gnuplot -e "set term png; set output 'lorentz.png';		\
		plot 'freq_lorentz' with lines, 1/(pi*(1+x**2));	\
		set output 'gauss.png';					\
		plot 'freq_gauss' with lines, exp(-x**2/2)/sqrt(2*pi)"

report:	assgn5.tex
	xelatex assgn5

clean:
	rm assgn5.{aux,log,pdf} a.out freq_{lorentz,gauss}		\
		{lorentz,gauss}.png gauss_efficiency
