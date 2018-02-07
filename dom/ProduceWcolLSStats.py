#!/usr/bin/env python

#./ProduceWcolLSStats.py ../networks/medium_LS_rep.csv > ../networks/medium_LS_stats.csv

import pandas as pd
import sys
import math

filename = sys.argv[1]
df = pd.read_csv(filename)
cols =  df.columns
progs = cols[2:]
progs_cnt = len(progs) / 2
before = []
after = []
names = []
suffix_to_cut = "_wcol"
for prog_id in range(progs_cnt):
  before.append(progs[2 * prog_id])
  after.append(progs[2 * prog_id + 1])
  names.append(progs[2 * prog_id][:-len(suffix_to_cut)])

idx = df.index
rad_col = df['rad']
max_rad = rad_col[0]
for rad in rad_col:
  max_rad = max(max_rad, rad)
assert(len(idx) % (max_rad - 1) == 0)
sum_log = [[0 for _ in range(progs_cnt)] for _ in range(max_rad + 1)]
sum_rat = [[0 for _ in range(progs_cnt)] for _ in range(max_rad + 1)]
cnt_data = [[0 for _ in range(progs_cnt)] for _ in range(max_rad + 1)]

for row_id in idx:
  rad = df['rad'][row_id]
  if rad == 1:
    continue
  for prog_id in range(progs_cnt):
    prog_bef = before[prog_id]
    prog_aft = after[prog_id]
    res_bef = df[prog_bef][row_id]
    res_aft = df[prog_aft][row_id]
    ratio = 1. * res_aft / res_bef
    sum_log[0][prog_id] += math.log(res_aft)
    sum_log[rad][prog_id] += math.log(res_aft)
    sum_rat[0][prog_id] += ratio
    sum_rat[rad][prog_id] += ratio
    cnt_data[0][prog_id] += 1
    cnt_data[rad][prog_id] += 1

sys.stdout.write('avg_improvement,')
for prog_id in range(progs_cnt):
  sys.stdout.write(names[prog_id])
  if prog_id != progs_cnt - 1:
    sys.stdout.write(',')
print("")
for rad in range(max_rad + 1):
  if rad == 1:
    continue
  if rad == 0:
    sys.stdout.write("all,")
  else:
    sys.stdout.write(str(rad) + ",")
  for prog_id in range(progs_cnt):
    improvement = 1 - 1. * sum_rat[rad][prog_id] / cnt_data[rad][prog_id];
    sys.stdout.write(str("%.1f"%(100 * improvement)) + "%")
    if prog_id != progs_cnt - 1:
      sys.stdout.write(',')
  print ("")

print("")

sys.stdout.write('avg_logs,')
for prog_id in range(progs_cnt):
  sys.stdout.write(names[prog_id])
  if prog_id != progs_cnt - 1:
    sys.stdout.write(',')
print("")
for rad in range(max_rad + 1):
  if rad == 1:
    continue
  if rad == 0:
    sys.stdout.write("all,")
  else:
    sys.stdout.write(str(rad) + ",")
  for prog_id in range(progs_cnt):
    sys.stdout.write(str("%.3f"%(1. * sum_log[rad][prog_id] / cnt_data[rad][prog_id])))
    if prog_id != progs_cnt - 1:
      sys.stdout.write(',')
  print ("")

print("")

sys.stdout.write('progs_apx_ratio,')
  
best_freq = [[0 for _ in range(progs_cnt)] for _ in range(max_rad + 1)]
apx_ratio = [[0 for _ in range(progs_cnt)] for _ in range(max_rad + 1)]
for row_id in idx:
  rad = df['rad'][row_id]
  best = 10**8
  for prog_id in range(progs_cnt):
    prog = after[prog_id]
    best = min(best, df[prog][row_id])
  cnt_best = 0
  for prog_id in range(progs_cnt):
    prog = after[prog_id]
    if best == df[prog][row_id]:
      cnt_best += 1
  for prog_id in range(progs_cnt):
    prog = after[prog_id]
    if rad != 1:
      apx_ratio[0][prog_id] += 1. * df[prog][row_id] / best
      if df[prog][row_id] == best:
        best_freq[0][prog_id] += 1. / cnt_best
    apx_ratio[rad][prog_id] += 1. * df[prog][row_id] / best
    if df[prog][row_id] == best:
      best_freq[rad][prog_id] += 1. / cnt_best
      

for prog_id in range(progs_cnt):
  sys.stdout.write(after[prog_id])
  if prog_id != progs_cnt - 1:
    sys.stdout.write(',')
print("")
for rad in range(max_rad + 1):
  if rad == 1:
    continue
  if rad == 0:
    sys.stdout.write("all,")
  else:
    sys.stdout.write(str(rad) + ",")
  for prog_id in range(progs_cnt):
    sys.stdout.write(str("%.3f"%(1. * apx_ratio[rad][prog_id] / cnt_data[rad][prog_id])))
    if prog_id != progs_cnt - 1:
      sys.stdout.write(',')
  print ("")

print ("")

sys.stdout.write('progs_win_score,')

for prog_id in range(progs_cnt):
  sys.stdout.write(after[prog_id])
  if prog_id != progs_cnt - 1:
    sys.stdout.write(',')
print("")
for rad in range(max_rad + 1):
  if rad == 1:
    continue
  if rad == 0:
    sys.stdout.write("all,")
  else:
    sys.stdout.write(str(rad) + ",")
  for prog_id in range(progs_cnt):
    sys.stdout.write(str("%.3f"%(1. * best_freq[rad][prog_id])))
    if prog_id != progs_cnt - 1:
      sys.stdout.write(',')
  print ("")

  