for input in *.txtg; do
  name=${input:0:-5}
  ~/Dokumenty/Mgr/mmmmm/compute_wcol_order -i $input -o orders/$name.christoph.txt -m 0
done
    