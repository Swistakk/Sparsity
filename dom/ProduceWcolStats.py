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
  if rad == 1:
    continue
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
  if rad == 1:
    continue
  if rad == 0:
    sys.stdout.write("all,")
  else:
    sys.stdout.write(str(rad) + ",")
  for prog_id in range(len(progs)):
    sys.stdout.write(str("%.3f"%(1. * sum_log[rad][prog_id] / cnt_data[rad][prog_id])))
    if prog_id != len(progs) - 1:
      sys.stdout.write(',')
  print ("")

berlins_cnt = 19
felix_cnt = 5

for table_ind in range(0, 3):
  print ("")
  if table_ind == 0:
    sys.stdout.write('berlins_apx_ratio,')
    progs_in_table = cols[-berlins_cnt:]
  elif table_ind == 1:
    sys.stdout.write('felixs_apx_ratio,')
    progs_in_table = cols[-(berlins_cnt + felix_cnt):-berlins_cnt]
  else:
    sys.stdout.write('progs_apx_ratio,')
    progs_in_table = progs
  
  best_freq = [[0 for _ in range(len(progs_in_table))] for _ in range(max_rad + 1)]
  apx_ratio = [[0 for _ in range(len(progs_in_table))] for _ in range(max_rad + 1)]
  for row_id in idx:
    rad = df['rad'][row_id]
    best = 10**8
    for prog_id in range(len(progs_in_table)):
      prog = progs_in_table[prog_id]
      best = min(best, df[prog][row_id])
    cnt_best = 0
    for prog_id in range(len(progs_in_table)):
      prog = progs_in_table[prog_id]
      if best == df[prog][row_id]:
        cnt_best += 1
    for prog_id in range(len(progs_in_table)):
      prog = progs_in_table[prog_id]
      if rad != 1:
        apx_ratio[0][prog_id] += 1. * df[prog][row_id] / best
        if df[prog][row_id] == best:
          best_freq[0][prog_id] += 1. / cnt_best
      apx_ratio[rad][prog_id] += 1. * df[prog][row_id] / best
      if df[prog][row_id] == best:
        best_freq[rad][prog_id] += 1. / cnt_best
        
  
  for prog_id in range(len(progs_in_table)):
    sys.stdout.write(progs_in_table[prog_id])
    if prog_id != len(progs_in_table) - 1:
      sys.stdout.write(',')
  print("")
  for rad in range(max_rad + 1):
    if rad == 1:
      continue
    if rad == 0:
      sys.stdout.write("all,")
    else:
      sys.stdout.write(str(rad) + ",")
    for prog_id in range(len(progs_in_table)):
      sys.stdout.write(str("%.3f"%(1. * apx_ratio[rad][prog_id] / cnt_data[rad][prog_id])))
      if prog_id != len(progs_in_table) - 1:
        sys.stdout.write(',')
    print ("")
  
  print ("")
  if table_ind == 0:
    sys.stdout.write('berlins_win_score,')
  elif table_ind == 1:
    sys.stdout.write('felixs_win_score,')
  else:
    sys.stdout.write('progs_win_score,')
  
  for prog_id in range(len(progs_in_table)):
    sys.stdout.write(progs_in_table[prog_id])
    if prog_id != len(progs_in_table) - 1:
      sys.stdout.write(',')
  print("")
  for rad in range(max_rad + 1):
    if rad == 1:
      continue
    if rad == 0:
      sys.stdout.write("all,")
    else:
      sys.stdout.write(str(rad) + ",")
    for prog_id in range(len(progs_in_table)):
      sys.stdout.write(str("%.3f"%(1. * best_freq[rad][prog_id])))
      if prog_id != len(progs_in_table) - 1:
        sys.stdout.write(',')
    print ("")
  
  