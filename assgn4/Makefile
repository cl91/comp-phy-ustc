all:	main.c marsaglia.c assgn4.tex
	cc -Wall main.c -lm
	./a.out > results
	gnuplot -e "set term png; set output 'sphere.png'; plot 'results'"
	cc -Wall marsaglia.c -lm
	./a.out > res_marsa
	gnuplot -e "set term png; set output 'sph_marsa.png'; plot 'res_marsa'"
	xelatex assgn4

clean:
	rm sphere.png sph_marsa.png a.out results res_marsa assgn4.{aux,log}
