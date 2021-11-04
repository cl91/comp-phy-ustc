#!/usr/bin/python2

import subprocess

file = "res"

xvalues = [int(line.split()[0]) for line in open(file)]
yvalues = [int(line.split()[1]) for line in open(file)]

plotarg = "\"set term png; set output 'res.png';"                    \
    + "plot [" + str(min(xvalues)-2) + ":" + str(max(xvalues)+2)     \
    + "] [" + str(min(yvalues)-2) + ":" + str(max(yvalues)+2)        \
    + "] 'res' with l\""

subprocess.call('gnuplot -e ' + plotarg, shell=True)
subprocess.call('gnuplot -e "set term png; set output \'freq.png\';'
		+ 'plot \'freq\' with l"', shell=True)
subprocess.call('gnuplot -e "set term png; set output \'exp.png\';'
                + 'fit a*x+b \'exp\' via a, b;'
                + 'plot \'exp\', a*x+b;'
                + 'set print \'res_exp\';'
                + 'print a/2"', shell=True)
