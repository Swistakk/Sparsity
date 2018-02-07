./execute.sh sortdeg1.sh $1
./execute.sh LSsortdeg1.sh $1
(./execute.sh ./generate_csv_wcol_LSsortdeg1.sh $1) > $1_rep_LSsortdeg1.csv
