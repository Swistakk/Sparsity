single_timeout=300
for input in *.txtg; do
  name=${input:0:-5}
  echo $name
  /usr/bin/time -q -f "%E" timeout $single_timeout ~/Dokumenty/Mgr/dom/min-fill-in/bin/apprTW -i $input -o > orders/$name.fill-in.txt
done
    