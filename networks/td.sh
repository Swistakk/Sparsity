single_timeout=300
for input in *.txtg; do
  name=${input:0:-5};
  echo $input
  /usr/bin/time -q -f "%E" timeout $single_timeout python3.5 ~/Dokumenty/Mgr/td/td.py $input > orders/$name.td.txt
done
    