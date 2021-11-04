#!/bin/bash

for i in res_*;	do
    gnuplot -e "set term png;			\
		set output 'plot${i#res}.png';	\
		plot '${i}' with l"
    done
