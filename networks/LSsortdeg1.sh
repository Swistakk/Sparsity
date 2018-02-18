TIMEOUT=60
for input in *.txtg; do
  for rad in {1..5}; do
    name=${input:0:-5};
    progs="sortdeg1"
    for prog in $progs; do
      echo $prog;
      echo "../../dom/LSWcolBoth --in_g=$input --in_o=orders/$name.$prog.txt --rad=$rad --T=$TIMEOUT"
      ../../dom/LSWcolBoth --in_g=$input --in_o=orders/$name.$prog.txt --rad=$rad --T=$TIMEOUT
    done
  done
done
    
