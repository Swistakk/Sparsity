# ./generate_rep_LS.sh medium
cd $1

echo "test,rad,sortdeg1_wcol,sortdeg1_LS,berlin.dna_wcol,berlin.dna_LS,felixR_wcol,felixR_LS" > ../$1_LS_rep.csv

for rad in {2..5}; do
  for input in *.txtg; do
    name=${input:0:-5};
    felix="felix"$rad
    progs="sortdeg1 berlin.dna "$felix
    printf $input,$rad >> ../$1_LS_rep.csv;
    for prog in $progs; do
      before=$( (~/Dokumenty/Mgr/dom/WcolStats --in_g=$input --in_o=orders/$name.$prog.txt --rad=$rad --mode=wcol) | tr -d '\n')
      after=$( (~/Dokumenty/Mgr/dom/WcolStats --in_g=$input --in_o=orders/$name.$prog.ls$rad.txt --rad=$rad --mode=wcol) | tr -d '\n')
      printf ,$before,$after >> ../$1_LS_rep.csv;
    done
    printf '\n' >> ../$1_LS_rep.csv;
  done
done;
cd ..
