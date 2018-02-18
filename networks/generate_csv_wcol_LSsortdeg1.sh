#(./execute.sh ./generate_csv_wcol_LSsortdeg1.sh small) > small_rep.csv
printf test,rad,sortdeg1.lsR;
printf "\n"
for rad in {1..5}; do
  for input in *.txtg; do
    name=${input:0:-5};
    progs="sortdeg1.ls"$rad
    last_prog="sortdeg1.ls"$rad
    printf $name,$rad,
    (for prog in $progs; do
      ~/git/programs/wcol-practical/dom/WcolStats --in_g=$input --in_o=orders/$name.$prog.txt --rad=$rad --mode=wcol;
      if [ $prog != $last_prog ]
      then
        printf ,
      fi
    done) | tr -d '\n'
    printf "\n"
  done
done
    