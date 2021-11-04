#!/bin/bash

for x in res_*; do
    gnuplot -e "set term png;set output '"$x".png';plot [0:256] [0:256] '"$x"'"
done

for x in res_*.png; do
    echo "\begin{center}" >> res_plot.tex
    echo "\includegraphics[width=4in]{"${x}"}" >> res_plot.tex
    echo "\end{center}" >> res_plot.tex
done
