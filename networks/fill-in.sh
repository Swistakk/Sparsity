for input in *.txtg; do
  name=${input:0:-5}
  echo $name
  ~/Dokumenty/Mgr/dom/min-fill-in/bin/apprTW -i $input -o > $name.fill-in.txt
done
    