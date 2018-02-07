#! /usr/bin/env python3

#python3.5 ./generate_rep_with_times.py small

import argparse
import subprocess
import os
import sys

parser = argparse.ArgumentParser()
parser.add_argument('dir', type=str)
args = parser.parse_args()

single_timeout = 60
timetmp = "mktemp2"
outtmp = "tmp2"
inputs = os.popen("cd " + args.dir + " && ls *.txtg").read()
inputs = inputs.split()
print (inputs)

if os.path.isfile(outtmp):
  print ("There can be no \"" + outtmp + "\" file in your directory")
  exit(1)

os.system("touch " + outtmp)

progs = ["sortdeg1", "td", "fill-in", "berlin.sna", "felixR"]

max_rad = 5
res = [[[0 for ind in range(len(progs))] for rad in range(max_rad + 1)] for input in inputs]
timestamp = [[[0 for ind in range(len(progs))] for rad in range(max_rad + 1)] for input in inputs]

for prog_id in range(len(progs)):
  sys.stderr.write("Running {}\n".format(progs[prog_id]))
  for input_ind in range(len(inputs)):
    input = inputs[input_ind]
    input_with_dir = args.dir + "/" + input
    str_st = str(single_timeout)
    if prog_id == 0:
      command = "/usr/bin/time -q --output=" + timetmp + " -f \"%E\" timeout " + str_st + " ../dom/SortDeg --in=" + input_with_dir + " --rad=1 --o=" + outtmp
    elif prog_id == 1:
      command = "/usr/bin/time -q --output=" + timetmp + " -f \"%E\" timeout " + str_st + " python3.5 ../td/td.py " + input_with_dir + " > " + outtmp
    elif prog_id == 2:
      command = "/usr/bin/time -q --output=" + timetmp + " -f \"%E\" timeout " + str_st + " ../dom/min-fill-in/bin/apprTW -i " + input_with_dir + " -o > " + outtmp
    elif prog_id == 3:
      command = "/usr/bin/time -q --output=" + timetmp + " -f \"%E\" timeout " + str_st + " ../dom/BerlinWcol --in=" + input_with_dir + " --order=sortdeg --rev=no --rule=all --o=" + outtmp
    
    if prog_id <= 3:
      ret = os.system(command) // 256
    
    was_timeout_or_rte = False
    for rad in range(2, max_rad + 1):
      if prog_id == 4 and (not was_timeout_or_rte):
        ret = os.system("/usr/bin/time -q --output=" + timetmp + " -f \"%E\" timeout " + str_st + " ../augmental/scripts/dtf_to_file.py " + input_with_dir + " " + str(rad) + " " + outtmp) // 256
      if ret == 124:
        timestamp[input_ind][rad][prog_id] = "TIMEOUT"
        res[input_ind][rad][prog_id] = -1
        was_timeout_or_rte = True
      elif ret == 0:
        timestamp[input_ind][rad][prog_id] = os.popen("cat " + timetmp + " | tr \"\n\" \" \"").read()
        res[input_ind][rad][prog_id] = os.popen("(../dom/WcolStats --in_g=" + input_with_dir + " --in_o=" + outtmp + " --rad=" + str(rad) + " --mode=wcol) | tr -d '\n'").read()
      else:
        timestamp[input_ind][rad][prog_id] = "RTE"
        res[input_ind][rad][prog_id] = -1
        was_timeout_or_rte = True

output_name = args.dir + "_rep_with_times.csv"
out = open(output_name, "w")
out.write("test,rad,sortdeg1_wcol,sortdeg1_time,td_wcol,td_time,fill-in_wcol,fill-in_time,berlin.sna_wcol,berlin.sna_time,felixR_wcol,felixR_time\n")


for rad in range(2, max_rad + 1):
  for input_ind in range(len(inputs)):
    input = inputs[input_ind]
    out.write(input[:-5] + "," + str(rad))
    for prog_id in range(len(progs)):
      out.write("," + str(res[input_ind][rad][prog_id]) + "," + str(timestamp[input_ind][rad][prog_id]))
    out.write("\n")

os.system("rm " + outtmp)
