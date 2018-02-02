for input in *.txtg; do
  for order in bfs dfs sortdeg; do
    for revv in no yes; do
      for rule in all neis_of_past neis_in_past; do
        ~/Dokumenty/Mgr/dom/BerlinWcol --in=$input --order=$order --rev=$revv --rule=$rule;
      done
    done;
  done;
#   for rad in {1..5}; do
#   done;
done
    