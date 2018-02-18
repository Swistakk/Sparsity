#! /usr/bin/env python3

import glob
import os
from collections import defaultdict

def compute_stats(filename):
    degrees = defaultdict(int)
    edges = set()
    with open(filename, 'r') as filebuf:
        for l in filebuf:
            if l[0] in ['#', '%']:
                continue
            s,t = map(int, l[:-1].split())
            if (s,t) in edges:
                continue
            edges.add((s,t))
            degrees[s] += 1
            degrees[t] += 1
    n = len(degrees)
    m = len(edges)
    avg_deg = 2*m / n
    max_deg = max(degrees.values())
    return n, m, avg_deg, max_deg


files = glob.glob('small/*.txtg')
files += glob.glob('medium/*.txtg')
files += glob.glob('large/*.txtg')
files += glob.glob('huge/*.txtg')

print("test;n;m;avg_deg;max_deg")
for f in files:
    name, _ = os.path.splitext(os.path.basename(f))
    n, m, avg_deg, max_deg = compute_stats(f)
    print("{};{};{};{:.2f};{}".format(name,n,m,avg_deg,max_deg))
