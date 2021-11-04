all:	code report

code:	main.c
	cc -Wall main.c -lm
	./a.out
	gnuplot -e "set term png; set output 'markov_all.png';	\
		plot 'markov_all' with l"
	gnuplot -e "set term png; set output 'markov_hit.png';	\
		plot 'markov_hit' with l"

report:	assgn16.tex
	xelatex assgn16

clean:
	-rm assgn16.{pdf,aux,log} a.out markov_{all,hit}{,.png}
	-rm res_{x2,y2,x2_y2}
