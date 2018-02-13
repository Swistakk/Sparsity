for input in *.txtg; do
  name=${input:0:-5};
  for rad in {1..5}; do
    echo $name $rad
    for percentage in 100 20; do
      ~/Dokumenty/Mgr/dom/UQWMix $input $rad tree $percentage > UQW/$name.mix_tree$rad.$percentage.txt;
      ~/Dokumenty/Mgr/dom/UQWMix $input $rad tree_shrink $percentage > UQW/$name.mix_tree_shrink$rad.$percentage.txt;
      ~/Dokumenty/Mgr/dom/UQWMix $input $rad ld_it $percentage > UQW/$name.mix_ldit$rad.$percentage.txt;
      ~/Dokumenty/Mgr/dom/UQWMix $input $rad ld_pow $percentage > UQW/$name.mix_ldpow$rad.$percentage.txt;
    done
  done
done
    