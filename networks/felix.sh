for input in *.txtg; do
  for rad in {1..5}; do
    ~/Dokumenty/Mgr/augmental/scripts/dtf.py $input $rad;
  done;
done
    