#! /usr/bin/env python3

import glob
import os
from collections import defaultdict

def compute_stats(filename):
    degrees = defaultdict(int)
    edges = set()
    numerrors = 0
    with open(filename, 'r') as filebuf:
        for i, l in enumerate(filebuf):
            if l[0] in ['#', '%']:
                continue
            if l[-1] == '\n':
                l = l[:-1]
            try:
                s,t = map(int, l.split())
            except:
                numerrors += 1
                continue

            if (s,t) in edges:
                continue
            edges.add((s,t))
            degrees[s] += 1
            degrees[t] += 1
    if numerrors > 1: # Some files have trailing edge-count 
        raise Exception("Too many errors")
    n = len(degrees)
    m = len(edges)
    avg_deg = 2*m / n
    max_deg = max(degrees.values())
    return n, m, avg_deg, max_deg


files = glob.glob('*/*.txtg')

print("test;n;m;avg_deg;max_deg")
for f in files:
    name, _ = os.path.splitext(os.path.basename(f))
    try:
        n, m, avg_deg, max_deg = compute_stats(f)
        print("{};{};{};{:.2f};{}".format(name,n,m,avg_deg,max_deg))
    except (KeyboardInterrupt, SystemExit):
        raise
    except:
        print("Error in reading file", f)
        raise
