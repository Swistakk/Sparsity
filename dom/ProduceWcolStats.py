#!/usr/bin/env python

#./ProduceWcolStats.py ../networks/medium_rep.csv > ../networks/medium_stats.csv

import pandas as pd
import sys
import math

filename = sys.argv[1]
df = pd.read_csv(filename)
cols =  df.columns
progs = cols[2:]
idx = df.index
rad_col = df['rad']
max_rad = rad_col[0]
for rad in rad_col:
  max_rad = max(max_rad, rad)
assert(len(idx) % max_rad == 0)
sum_log = [[0 for _ in range(len(progs))] for _ in range(max_rad + 1)]
cnt_data = [[0 for _ in range(len(progs))] for _ in range(max_rad + 1)]

for row_id in idx:
  rad = df['rad'][row_id]
  for prog_id in range(len(progs)):
    prog = progs[prog_id]
    sum_log[0][prog_id] += math.log(df[prog][row_id])
    sum_log[rad][prog_id] += math.log(df[prog][row_id])
    cnt_data[0][prog_id] += 1
    cnt_data[rad][prog_id] += 1
    
sys.stdout.write('avg_logs,')
for prog_id in range(len(progs)):
  sys.stdout.write(progs[prog_id])
  if prog_id != len(progs) - 1:
    sys.stdout.write(',')
print("")
for rad in range(max_rad + 1):
  if rad == 0:
    sys.stdout.write("all,")
  else:
    sys.stdout.write(str(rad) + ",")
  for prog_id in range(len(progs)):
    sys.stdout.write(str("%.3f"%(1. * sum_log[rad][prog_id] / cnt_data[rad][prog_id])))
    if prog_id != len(progs) - 1:
      sys.stdout.write(',')
  print ("")

berlins = cols[-12:]
apx_ratio = [[0 for _ in range(len(berlins))] for _ in range(max_rad + 1)]
for row_id in idx:
  rad = df['rad'][row_id]
  best = 10**8
  for berlin_id in range(len(berlins)):
    berlin = berlins[berlin_id]
    best = min(best, df[berlin][row_id])
  for berlin_id in range(len(berlins)):
    berlin = berlins[berlin_id]
    apx_ratio[0][berlin_id] += 1. * df[berlin][row_id] / best
    apx_ratio[rad][berlin_id] += 1. * df[berlin][row_id] / best
print ("")
sys.stdout.write('berlins_apx_ratio,')
for berlin_id in range(len(berlins)):
  sys.stdout.write(berlins[berlin_id])
  if berlin_id != len(berlins) - 1:
    sys.stdout.write(',')
print("")
for rad in range(max_rad + 1):
  if rad == 0:
    sys.stdout.write("all,")
  else:
    sys.stdout.write(str(rad) + ",")
  for berlin_id in range(len(berlins)):
    sys.stdout.write(str("%.3f"%(1. * apx_ratio[rad][berlin_id] / cnt_data[rad][berlin_id])))
    if berlin_id != len(berlins) - 1:
      sys.stdout.write(',')
  print ("")
  
felixs = cols[-17:-12]
apx_ratio = [[0 for _ in range(len(berlins))] for _ in range(max_rad + 1)]
for row_id in idx:
  rad = df['rad'][row_id]
  best = 100**8
  for felix_id in range(len(felixs)):
    felix = felixs[felix_id]
    best = min(best, df[felix][row_id])
  for felix_id in range(len(felixs)):
    felix = felixs[felix_id]
    apx_ratio[0][felix_id] += 1. * df[felix][row_id] / best
    apx_ratio[rad][felix_id] += 1. * df[felix][row_id] / best
print("")
sys.stdout.write('felixs_apx_ratio,')
for felix_id in range(len(felixs)):
  sys.stdout.write(felixs[felix_id])
  if felix_id != len(felixs) - 1:
    sys.stdout.write(',')
print("")
for rad in range(max_rad + 1):
  if rad == 0:
    sys.stdout.write("all,")
  else:
    sys.stdout.write(str(rad) + ",")
  for felix_id in range(len(felixs)):
    sys.stdout.write(str("%.3f"%(1. * apx_ratio[rad][felix_id] / cnt_data[rad][felix_id])))
    if felix_id != len(felixs) - 1:
      sys.stdout.write(',')
  print("")

apx_ratio = [[0 for _ in range(len(progs))] for _ in range(max_rad + 1)]
for row_id in idx:
  rad = df['rad'][row_id]
  best = 10**8
  for prog_id in range(len(progs)):
    prog = progs[prog_id]
    best = min(best, df[prog][row_id])
  for prog_id in range(len(progs)):
    prog = progs[prog_id]
    apx_ratio[0][prog_id] += 1. * df[prog][row_id] / best
    apx_ratio[rad][prog_id] += 1. * df[prog][row_id] / best
print ("")
sys.stdout.write('progs_apx_ratio,')
for prog_id in range(len(progs)):
  sys.stdout.write(progs[prog_id])
  if prog_id != len(progs) - 1:
    sys.stdout.write(',')
print("")
for rad in range(max_rad + 1):
  if rad == 0:
    sys.stdout.write("all,")
  else:
    sys.stdout.write(str(rad) + ",")
  for prog_id in range(len(progs)):
    sys.stdout.write(str("%.3f"%(1. * apx_ratio[rad][prog_id] / cnt_data[rad][prog_id])))
    if prog_id != len(progs) - 1:
      sys.stdout.write(',')
  print ("")
 