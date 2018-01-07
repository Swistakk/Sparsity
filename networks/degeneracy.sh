for input in *.txtg; do
  for rad in {1..5}; do
    ~/Dokumenty/Mgr/dom/Degeneracy --in=$input --rad=$rad;
  done
done
    