# ./generate_rep_with_times.sh medium
cd $1
single_timeout=300
timetmp=`mktemp`

echo "test,rad,sortdeg1_wcol,sortdeg1_time,berlin.dna_wcol,berlin.dna_time,felixR_wcol,felixR_time" > ../$1_rep_with_times.csv

if [ -e tmp ]
then
  printf "There can be no \"tmp\" file in your directory"
  break;
fi

touch tmp

for rad in {2..5}; do
  for input in *.txtg; do
    name=${input:0:-5};
    /usr/bin/time -q --output=$timetmp -f "%E" timeout $single_timeout ../../dom/SortDeg --in=$input --rad=1 --o=tmp
    
    ret=$?
    if (( $ret == 124 )); then
      timestamp_sortdeg="TIMEOUT"
      res_sortdeg=-1
    elif (( $ret == 0)); then
      timestamp_sortdeg=`cat $timetmp | tr "\n" " "`
      res_sortdeg=$( (~/Dokumenty/Mgr/dom/WcolStats --in_g=$input --in_o=tmp --rad=$rad --mode=wcol) | tr -d '\n')
    else
      timestamp_sortdeg="RTE"
      res_sortdeg=-1
    fi
    /usr/bin/time -q --output=$timetmp -f "%E" timeout $single_timeout ../../dom/BerlinWcol --in=$input --order=dfs --rev=no --rule=all --o=tmp
    ret=$?
    if (( $ret == 124 )); then
      timestamp_berlin="TIMEOUT"
      res_berlin=-1
    elif (( $ret == 0)); then
      timestamp_berlin=`cat $timetmp | tr "\n" " "`
      res_berlin=$( (~/Dokumenty/Mgr/dom/WcolStats --in_g=$input --in_o=tmp --rad=$rad --mode=wcol) | tr -d '\n')
    else
      timestamp_berlin="RTE"
      res_berlin=-1
    fi
    #full_path=$(pwd)"tmp"
    /usr/bin/time -q --output=$timetmp -f "%E" timeout $single_timeout ~/Dokumenty/Mgr/augmental/scripts/dtf_to_file.py $input $rad tmp;
    ret=$?
    if (( $ret == 124 )); then
      timestamp_felix="TIMEOUT"
      res_felix=-1
    elif (( $ret == 0)); then
      timestamp_felix=`cat $timetmp | tr "\n" " "`
      res_felix=$( (~/Dokumenty/Mgr/dom/WcolStats --in_g=$input --in_o=tmp --rad=$rad --mode=wcol) | tr -d '\n')
    else
      timestamp_felix="RTE"
      res_felix=-1
    fi
    echo "$input,$rad,$res_sortdeg,$timestamp_sortdeg,$res_berlin,$timestamp_berlin,$res_felix,$timestamp_felix" >> ../$1_rep_with_times.csv
  #../../dom/BerlinWcol --in=$input --order=$order --rev=$revv --rule=$rule;
  done;
done;
rm tmp
cd ..
