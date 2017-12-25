for input in *.txtg; do
  #length=`expr length $input`;
  #let name_length=$length-5;
  #name=${input:0:$name_length};
  name=${input:0:-5};
  for rad in {1..5}; do
    ~/Dokumenty/Mgr/dom/WcolStats --in_g=$input --in_o=orders/$name.deg.txt --rad=$rad --mode=wcol --v;
    ~/Dokumenty/Mgr/dom/WcolStats --in_g=$input --in_o=orders/$name.sortdeg.txt --rad=$rad --mode=wcol --v;
    for order in b d; do
      for revv in n y; do
        ~/Dokumenty/Mgr/dom/WcolStats --in_g=$input --in_o=orders/$name.berlin.$order$revv.txt --rad=$rad --mode=wcol --v;
      done;
    done;
    ~/Dokumenty/Mgr/dom/WcolStats --in_g=$input --in_o=orders/$name.felix$rad.txt --rad=$rad --mode=wcol --v;
    ~/Dokumenty/Mgr/dom/WcolStats --in_g=$input --in_o=orders/$name.random.txt --rad=$rad --mode=wcol --v;  
    printf '\n'
  done;
done
    