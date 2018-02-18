#! /usr/bin/env python3

# python3 uruchom.py  [MFCS|TGV|TGV2|TGV3|tree|tree_shrink|ldit|ldpow] [test] [radius] [100|20]

import sys
import os
import tempfile

prog = sys.argv[1]
graph_file = sys.argv[2]
rad = sys.argv[3]
percentage = sys.argv[4]

g_format = ".txtg"
last_slash = 0
for i in range(len(graph_file)):
  if graph_file[i] == '/':
    last_slash = i
graph_dir = graph_file[:last_slash]
graph_name = graph_file[last_slash + 1 : -len(g_format)]
order_file = graph_dir + "/orders/" + graph_name + ".sortdeg1.txt";

print ("PROG=%s\nGRAPH=%s\nR=%d\n%%=%d" % (prog, graph_file, int(rad), int(percentage)))
print ("ORDER=%s" % order_file)

root = ".." #change it?
exe_name = "UQW-MFCS";
is_mix = False


if prog == "MFCS" or prog == "TGV" or prog == "TGV2" or prog == "TGV3":
  out_suf = prog
  command = "{}/dom/{} {} {} {} {}".format(root, exe_name, graph_file, order_file, rad, percentage)
else:
  if prog == "tree" or prog == "tree_shrink" or prog == "ldit" or prog == "ldpow":
    exe_name = "UQWMix"
    command = "{}/dom/{} {} {} {} {}".format(root, exe_name, graph_file, rad, prog, percentage)
  else:
    print ("Wrong name of program")


output_file = graph_dir + "/UQW/" + graph_name + "." + prog + rad + "." + percentage + ".txt"

command += " > " + output_file
print ("COMMAND=%s" % command)

timeout = 10 #???
tmpf = tempfile.NamedTemporaryFile()

command = "/usr/bin/time -q --output=" + tmpf.name + " -f \"%E\" timeout " + str(timeout) + " " + command

res = os.system(command)
print ("RETURN_CODE=%d" % int(res))
t = tmpf.readline().strip()
tmpf.close()
print("TIME=%s" % t)
print("------------------------")
