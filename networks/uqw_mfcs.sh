for input in *.txtg; do
  name=${input:0:-5};
  for rad in {1..5}; do
    for percentage in 100 20; do
      ~/Dokumenty/Mgr/dom/UQW-MFCS $input orders/$name.sortdeg1.txt $rad $percentage > UQW/$name.MFCS$rad.$percentage.txt;
    done
  done
done
    