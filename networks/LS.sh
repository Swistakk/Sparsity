TIMEOUT=60
for input in *.txtg; do
  for rad in {2..5}; do
    name=${input:0:-5};
    felix="felix"$rad
    progs="sortdeg1 berlin.sna td fill-in "$felix
    for prog in $progs; do
      echo $prog;
      ../../dom/LSWcolBoth --in_g=$input --in_o=orders/$name.$prog.txt --rad=$rad --T=$TIMEOUT
    done
  done
done
    