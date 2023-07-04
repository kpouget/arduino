#! /usr/bin/env python3

import sys
import scipy.fft
import datetime

x = []
y = []
z = []
time = []

current_time = datetime.datetime.now()
POINT_OFFSET = datetime.timedelta(milliseconds=200)

with open("example.data") as f:
    for line in f.readlines():
        if line == "Go!\n": continue
        _x, _y, _z = [int(e) for e in line.split()]
        x.append(_x)
        y.append(_y)
        z.append(_z)
        time.append(current_time)
        current_time += POINT_OFFSET
