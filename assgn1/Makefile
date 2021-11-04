COMMON_SRC = schrage.c randomz.c
COMMON_HEADER = schrage.h randomz.h
X_Y_SRC = generate_x_y_dataset.c
X_Y_EXE = generate_x_y_dataset
TEST_SRC = test_x_k_c_l.c
TEST_EXE = test_x_k_c_l

all:	x_y test

x_y:	$(COMMON_SRC) $(COMMON_HEADER) $(X_Y_SRC)
	cc -Wall -o $(X_Y_EXE) $(COMMON_SRC) $(X_Y_SRC) -lm
	./$(X_Y_EXE) 1000000
	gnuplot plot.gnuplot
	mv schrage.png schrage_1e6.png
	mv randomz.png randomz_1e6.png
	./$(X_Y_EXE) 10000
	gnuplot plot.gnuplot
	mv schrage.png schrage_1e4.png
	mv randomz.png randomz_1e4.png

test:	$(COMMON_SRC) $(COMMON_HEADER) $(TEST_SRC)
	cc -Wall -o $(TEST_EXE) $(COMMON_SRC) $(TEST_SRC) -lm
	./$(TEST_EXE) > test_results
	cd report ; xelatex assgn1.tex

clean:
	rm $(X_Y_EXE) *.o $(TEST_EXE)				\
		schrage_{1e4,1e6}.png randomz_{1e4,1e6}.png	\
		result_{schrage,randomz}			\
		test_results
