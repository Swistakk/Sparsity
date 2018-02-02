for input in *.txtg; do
  name=${input:0:-5};
  echo $input
  python3.5 ~/Dokumenty/Mgr/td/td.py $input > orders/$name.td.txt
done
    