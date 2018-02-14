#(./execute.sh ./generate_csv_uqw100.sh small) > small_uqw100_rep.csv
progs="TGV TGV2 TGV3 MFCS tree tree_shrink ldit ldpow";
last_prog="ldpow"
printf test,rad,;
for prog in $progs; do
  printf $prog.forb,$prog.scat,$prog.score
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
      ~/Dokumenty/Mgr/dom/VerifierUQW $input UQW/$name.$prog$rad.100.txt $rad;
      if [ $prog != $last_prog ]
      then
        printf ,
      fi
    done) | tr -d '\n'
    printf "\n"
  done
done
    