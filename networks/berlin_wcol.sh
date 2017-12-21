for input in *.txtg; do
  for order in bfs dfs; do
    for revv in no yes; do
      for rule in all neis_of_past neis_in_past; do
        ~/Dokumenty/Mgr/dom/BerlinWcol $input --order=$order --rev=$revv --rule=$rule;
      done
    done;
  done;
#   for rad in {1..5}; do
#   done;
done
    