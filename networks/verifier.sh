for input in *.txtg; do
  #length=`expr length $input`;
  #let name_length=$length-5;
  #name=${input:0:$name_length};
  name=${input:0:-5};
  for rad in {1..5}; do
    #if [ $rad -eq 1 ]
    #then 
    ~/Dokumenty/Mgr/dom/VerifierWcol $input orders/$name.deg.txt $rad;
    #fi
    ~/Dokumenty/Mgr/dom/VerifierWcol $input orders/$name.sortdeg.txt $rad;
    ~/Dokumenty/Mgr/dom/VerifierWcol $input orders/$name.berlin.txt $rad;
    ~/Dokumenty/Mgr/dom/VerifierWcol $input orders/$name.berlinfixed.txt $rad;
    ~/Dokumenty/Mgr/dom/VerifierWcol $input orders/$name.felix$rad.txt $rad;
    printf '\n'
  done;
done
    