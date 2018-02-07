for input in *.txtg; do
  for rad in {1..5}; do
    ~/Dokumenty/Mgr/dom/SortDeg --in=$input --rad=$rad;
  done;
done
    