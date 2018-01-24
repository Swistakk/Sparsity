for input in *.txtg; do
  name=${input:0:-5};
  for rad in {1..5}; do
    ~/Dokumenty/Mgr/dom/UQWMix $input $rad tree > UQW/$name.mix_tree$rad.txt;
    ~/Dokumenty/Mgr/dom/UQWMix $input $rad ld_it > UQW/$name.mix_ldit$rad.txt;
    ~/Dokumenty/Mgr/dom/UQWMix $input $rad ld_pow > UQW/$name.mix_ldpow$rad.txt;
  done
done
    