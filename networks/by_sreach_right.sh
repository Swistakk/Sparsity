for input in *.txtg; do
  for rad in {1..5}; do
    #name=${input:0:-5};
    #diff -bsq orders/$name.bywreach$rad.txt orders/$name.by2wreach$rad.txt 
    #rm orders/$name.by2wreach$rad.txt
    ~/Dokumenty/Mgr/OldRepo/dom/BySReachRight --in=$input --rad=$rad;
  done;
done
    