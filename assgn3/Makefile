all:	main.c assgn3.tex
	cc -Wall main.c -lm
	./a.out > results
	gnuplot plot.gnuplot
	xelatex assgn3

clean:
	rm assgn3.{aux,log,pdf} results sphere.png a.out
