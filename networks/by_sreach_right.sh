single_timeout=300
for input in *.txtg; do
  for rad in {1..5}; do
    /usr/bin/time -q -f "%E" timeout $single_timeout ../../dom/BySReachRight --in=$input --rad=$rad;
  done;
done
    