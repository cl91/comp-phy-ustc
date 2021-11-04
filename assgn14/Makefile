all:	main.c
	make clean
	cc -Wall main.c -lm
	./a.out
	./plot.sh
	xelatex assgn14

clean:
	-rm res* a.out assgn14.{aux,log,pdf}
