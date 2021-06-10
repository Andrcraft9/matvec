import numpy as np
import sys

name = sys.argv[1]
#print("Read file: %s" % (name))
t1 = -1.0
with open(name) as f:
    print("#p th T S")
    line = f.readline()
    line = f.readline()
    while line:
        lsplt = line.split()
        
        p  = int(float(lsplt[3].split('=')[1]))
        th = int(float(lsplt[4].split('=')[1]))
        t  =     float(lsplt[8].split('=')[1])
        
        if (t1 < 0.0):
            t1 = t
        speedup = t1 / t
        print(p, th, t, speedup)
        line = f.readline()