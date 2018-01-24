#! /usr/bin/env python3

import sys
import subprocess
from collections import defaultdict

if len(sys.argv) < 2:
    print("Usage: ")
    sys.exit()

infile = sys.argv[1]
print("Input file {}".format(infile))

edges = []
nodes = set()   
with open(infile, 'r') as filebuf:
    for l in filebuf:
        u,v = l[:-1].split()
        u,v = int(u)+1, int(v)+1
        if u == v:
            continue 
        edges.append((u,v))
        nodes.add(u)
        nodes.add(v)
apex = max(nodes) + 1
for u in nodes:
    edges.append((apex,u))
nodes.add(apex)
print("Added apex with id {}".format(apex))

print("Converting into dimacs format")
with open('tmp.dimacs', 'w') as filebuf:
    filebuf.write("c Converted from {}\n".format(infile))
    filebuf.write("p edge {} {}\n".format(len(nodes), len(edges)))
    for u,v in edges:
        filebuf.write("e {} {}\n".format(u,v))

print("Running td heuristic")
# Probably should use flag "-s" for large graphs.
subprocess.run(["td-heuristic/td-bs", "-s", "-p", "-n", "tmp.dimacs"])

tchildren = defaultdict(list)
with open('tmp.dimacs', 'r') as filebuf:
    fileit = iter(filebuf)
    for l in fileit:
        if l[0] != "c":
            continue
        if l.startswith("c Treedepth"):
            break
    for l in fileit:
        assert l[0] == "t"
        _,u,v = l.split()
        u,v = int(u), int(v)
        nodes.remove(v)
        tchildren[u].append(v)
assert len(nodes) == 1
root = next(iter(nodes))
print("Root is {}".format(root))

visited = nodes
queue = [root]
order = []
while len(queue) > 0:
    u = queue.pop()
    order.append(u)
    queue += tchildren[u]
    visited.add(u)
order.remove(apex)

with open('tmp.order', 'w') as filebuf:
    for u in order:
        filebuf.write(str(u-1))
        filebuf.write(" ")

# This was for only. 
# Usage: --in_g=graph.txtg --in_o=order.txt --rad=radius --mode=wcol/sizes/full [--v]
# print("Running wcol stats")
# print(infile)
# for r in range(1,6):
#    subprocess.run(["WcolStats", "--in_g={}".format(infile), "--in_o=tmp.order", "--rad={}".format(r), "--mode=wcol"])
