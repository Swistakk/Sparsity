for input in *.txtg; do
  if [ -e tmp ]
  then
    break;
  fi
  tr ' ' '\t' < $input > tmp
  cp tmp $input
  rm tmp
done;