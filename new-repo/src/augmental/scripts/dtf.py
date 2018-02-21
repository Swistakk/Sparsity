#!/usr/bin/env python3

import sys, os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from dtf.graph import *
from dtf.graphformats import load_graph as load_graph

from collections import defaultdict
import itertools
import argparse

def avg(values):
    return sum(values) / len(values)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("graph", help="filename of the graph", type=str)
    parser.add_argument("d", help="max distance", type=int )
    args = parser.parse_args()

    filename = args.graph

    graphname = os.path.splitext( os.path.basename(filename))[0]
    g = load_graph(filename)
    g.remove_loops()
    
    args.d -= 1
    augg = DTFGraph.from_graph(g)
    if args.d == 0:
        res, seen = OGraph.ldo(g, True)
    else:
      d = 1
      while d <= args.d:
          augg.dtf_step(d+1) 
          d += 1

          in_degs = augg.in_degree_sequence()

      nowy, seen = OGraph.ldo(Graph.from_directed(augg), True)
    print (graphname)
    cur_dir = os.path.dirname(filename)
    if cur_dir == '':
      cur_dir = '.'
    ostream = open(cur_dir + "/orders/" + graphname + ".felix" + str(args.d + 1) + ".txt", "w")
    for v in seen:
      ostream.write(str(v))
      ostream.write(' ')
    ostream.write('\n')
    