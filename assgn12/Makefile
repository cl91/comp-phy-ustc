all:	main.c
	cc -Wall main.c -lm
	./a.out
	cc -Wall logistic.c -lm
	./a.out
	gnuplot -e "set term png; set output 'state.png';	\
			plot 'state' with dots"
	gnuplot -e "set term png; set output 'lyapunov.png';	\
			plot 'lyapunov' with l"
	gnuplot -e "set term png; set output 'state_logistic.png';	\
			plot 'state_logistic' with dots"
	gnuplot -e "set term png; set output 'lyapunov_logistic.png';	\
			plot 'lyapunov_logistic' with l"
	xelatex assgn12

clean:
	rm state{,.png} a.out assgn12.{log,aux,pdf} lyapunov{,.png}
	rm state_logistic{,.png} lyapunov_logistic{,.png}
