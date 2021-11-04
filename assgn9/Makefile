all:	code report

code:
	cc -Wall count_freq.c biased.c -lm
	./a.out
	cc -Wall count_freq.c unbiased.c -lm
	./a.out
	./run_plot.sh

report:
	xelatex assgn9
	xelatex assgn9

clean:
	rm a.out plot_* res_* assgn9.{aux,log,pdf}
