for input in *.txtg; do
  name=${input:0:-5};
  for rad in {1..5}; do
    ~/Dokumenty/Mgr/dom/UQW-MFCS $input orders/$name.sortdeg1.txt $rad > UQW/$name.MFCS$rad.txt;
  done
done
    