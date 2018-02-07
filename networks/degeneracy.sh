single_timeout=300
for input in *.txtg; do
  for rad in {1..5}; do
    /usr/bin/time -q -f "%E" timeout $single_timeout ~/Dokumenty/Mgr/dom/Degeneracy --in=$input --rad=$rad;
    ret=$?
    if (( $ret != 0 )); then
      break;
    fi
  done
done
    