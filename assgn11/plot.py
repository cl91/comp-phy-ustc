#!/usr/bin/python2

import subprocess

file = "res"

xvalues = [int(line.split()[0]) for line in open(file)]
yvalues = [int(line.split()[1]) for line in open(file)]

plotarg = "\"set term png; set output 'res.png';"                    \
    + "plot [" + str(min(xvalues)-2) + ":" + str(max(xvalues)+2)     \
    + "] [" + str(min(yvalues)-2) + ":" + str(max(yvalues)+2)        \
    + "] 'res' with points pt 3\""

subprocess.call('gnuplot -e ' + plotarg, shell=True)
