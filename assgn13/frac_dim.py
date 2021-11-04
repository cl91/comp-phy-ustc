#!/usr/bin/python

import math
import subprocess

# sandbox method
# count points within a rectangle of (2*w)*(2*w), centered at (x,y)
def count_points_rect(data, w, x, y):
    npoints = 0
    for v in data:
        if -w <= v[0]-x <= w and -w <= v[1]-y <= w:
            npoints += 1
    return npoints

# count points within a circle of radius r centered at (x,y)
def count_points_circle(data, r, x, y):
    npoints = 0
    for v in data:
        if (v[0] - x)**2 + (v[1] - y)**2 < r**2:
            npoints += 1
    return npoints

# plot the resulting log-log diagram and fit it linearly
def plot_gnuplot(fname):
    subprocess.call('gnuplot -e "set term png; set output \''
                    + fname + '.png\';'
                    + 'fit a*x+b \'' + fname +'\' via a,b;'
                    + 'plot \'' + fname + '\' with l, a*x+b;'
                    + 'set print \'' + fname + '_res\';'
                    + 'print a"', shell=True)

# compute the fractal dim. using sandbox method
def get_sandbox_dim(count_points, data, fname, x, y):
    with open(fname, "w") as f:
        for i in range(1, 70):
            f.write("%.12f %.12f\n" %
                    (math.log(i), math.log(count_points(data, i, x, y))))

        f.close()
    plot_gnuplot(fname)

# compute the radius of gyration for points within the circle of
# radius max_r centered at (x,y)
def get_radius_gyration(data, max_r, x, y):
    r_gy = 0.0
    npoints = 0
    for v in data:
        r2 = (v[0]-x)**2 + (v[1]-y)**2
        if r2 < max_r**2:
            r_gy += r2
            npoints += 1
    return npoints, math.sqrt(r_gy/npoints)

# compute the fractal dim. using radius of gyration method
def get_gyration_dim(data, fname, x, y):
    with open(fname, "w") as f:
        for r in range(10, 70):
            n, r = get_radius_gyration(data, r, x, y)
            f.write("%.12f %.12f\n" % (math.log(r), math.log(n)))
    plot_gnuplot(fname)

# get data from datafile
datafile = "dla"

cluster_dim = 1024
center_x = center_y = cluster_dim / 2

data = [[int(s) for s in line.split()] for line in open(datafile)]

# compute the fractal dimension with 2 methods
get_sandbox_dim(count_points_rect, data, 'sandbox_rect',
                center_x, center_y)
get_sandbox_dim(count_points_circle, data, 'sandbox_circle',
                center_x, center_y)
get_gyration_dim(data, 'gyration', center_x, center_y)
