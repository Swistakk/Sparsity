progs="random deg sortdeg felix1 felix2 felix3 felix4 felix5 berlin.bn berlin.dn berlin.by berlin.dy";
printf test,rad,;
for prog in $progs; do
  printf $prog,
done
printf "\n"
for rad in {1..5}; do
  for input in *.txtg; do
    name=${input:0:-5};
    printf $name,$rad,
    (for prog in $progs; do
      ~/Dokumenty/Mgr/dom/WcolStats $input orders/$name.$prog.txt $rad --mode=wcol;
      printf ,
    done) | tr -d '\n'
    printf "\n"
  done
done
    