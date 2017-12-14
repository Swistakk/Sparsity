for input in *.txtg; do
  for order in bfs dfs; do
    for revv in no yes; do
      ~/Dokumenty/Mgr/dom/BerlinWcol $input --order=$order --rev=$revv;
    done;
  done;
#   for rad in {1..5}; do
#   done;
done
    