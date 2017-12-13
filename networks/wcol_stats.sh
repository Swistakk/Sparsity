for input in *.txtg; do
  #length=`expr length $input`;
  #let name_length=$length-5;
  #name=${input:0:$name_length};
  name=${input:0:-5};
  for rad in {1..5}; do
    #if [ $rad -eq 1 ]
    #then 
    ~/Dokumenty/Mgr/dom/WcolStats $input orders/$name.deg.txt $rad --mode=wcol --v;
    #fi
    ~/Dokumenty/Mgr/dom/WcolStats $input orders/$name.sortdeg.txt $rad --mode=wcol --v;
    #~/Dokumenty/Mgr/dom/WcolStats $input orders/$name.berlin.txt $rad;
    ~/Dokumenty/Mgr/dom/WcolStats $input orders/$name.berlinfixed.txt $rad --mode=wcol --v;
    ~/Dokumenty/Mgr/dom/WcolStats $input orders/$name.felix$rad.txt $rad --mode=wcol --v;
    printf '\n'
  done;
done
    