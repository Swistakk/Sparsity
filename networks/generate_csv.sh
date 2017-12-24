berlins=""
for revv in n y; do
  for ord in b d; do
    for rule in a o i; do
      berlins="$berlins berlin.$ord$revv$rule";
    done
  done
done
progs="random deg sortdeg felix1 felix2 felix3 felix4 felix5 $berlins";
last_prog="berlin.dyi"
printf test,rad,;
for prog in $progs; do
  printf $prog
  if [ $prog != $last_prog ]
  then
    printf ,
  fi
done
printf "\n"
for rad in {1..5}; do
  for input in *.txtg; do
    name=${input:0:-5};
    printf $name,$rad,
    (for prog in $progs; do
      ~/Dokumenty/Mgr/dom/WcolStats $input orders/$name.$prog.txt $rad --mode=wcol;
      if [ $prog != $last_prog ]
      then
        printf ,
      fi
    done) | tr -d '\n'
    printf "\n"
  done
done
    