#(./execute.sh ./generate_csv_uqw.sh small) > small_uqw_rep.csv
progs="TGV TGV2 MFCS mix_tree mix_ldit mix_ldpow";
last_prog="mix_ldpow"
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
      ~/Dokumenty/Mgr/dom/VerifierUQW $input UQW/$name.$prog$rad.txt $rad;
      if [ $prog != $last_prog ]
      then
        printf ,
      fi
    done) | tr -d '\n'
    printf "\n"
  done
done
    