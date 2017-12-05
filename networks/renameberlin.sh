for input in *.txtg; do
  name=${input:0:-5};
  cp orders/$name.berlin1.txt orders/$name.berlin.txt;
  for rad in {1..5}; do
    rm orders/$name.berlin$rad.txt;
  done
done