# ./uqw_patch.sh medium
cd $1

output=../$1_uqw_patch_rep.csv

echo "test,dis,TGV.forb_bef,TGV.forb_aft,TGV.scat,TGV2.forb_bef,TGV2.forb_aft,TGV2.scat,MFCS.forb_bef,MFCS.forb_aft,MFCS.scat,ND.forb_bef,ND.forb_aft,ND.scat" > $output

for rad in {1..5}; do
  for input in *.txtg; do
    printf $input,$rad >> $output
    name=${input:0:-5};
    progs="TGV TGV2 MFCS ND"
    for prog in $progs; do
      ~/Dokumenty/Mgr/dom/UQWPatcher $input UQW/$name.$prog$rad.txt $rad
      before=$( ~/Dokumenty/Mgr/dom/VerifierUQW $input UQW/$name.$prog$rad.txt $rad )
      after=$( ~/Dokumenty/Mgr/dom/VerifierUQW $input UQW/$name.$prog$rad.p.txt $rad )
      comma=`expr index "$before" ,`
      forb_bef=${before:0:$comma-1};
      printf ,$forb_bef,$after >> $output
    done
    printf '\n' >> $output;
  done
done

cd ..
    