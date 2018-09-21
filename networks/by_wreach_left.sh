single_timeout=300
for input in *.txtg; do
  for rad in {1..5}; do
    #name=${input:0:-5};
    #diff -bsq orders/$name.bywreach$rad.txt orders/$name.by2wreach$rad.txt 
    #rm orders/$name.by2wreach$rad.txt
    /usr/bin/time -q -f "%E" timeout $single_timeout ~/Dokumenty/Mgr/OldRepo/dom/ByWReachLeft --in=$input --rad=$rad;
  done;
done
    