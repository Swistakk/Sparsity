declare -a graphs=("airlines"
                   "bergen-closeness"
                   "bergen"
                   "ca-CondMat"
                   "ca-HepPh"
                   "celegans"
                   "cit-HepPh"
                   "codeminer"
                   "cond-mat"
                   "cpan-authors"
                   "cpan-distributions"
                   "diseasome"
                   "dolphins"
                   "email-Enron"
                   "football"
                   "hep-th"
                   "hex"
                   "karate"
                   "lesmiserables"
                   "loc-brightkite_edges"
                   "netscience"
                   "photoviz_dynamic"
                   "polblogs"
                   "polbooks"
                   "power"
                   "soc-Epinions1"
                   "soc-Slashdot0811"
                   "southern-women"
                   "sp_data_school_day_2"
                   "twittercrawl"
                   "word_adjacencies"
                   "yeast")
declare -a formats=("leda" "txt" "gml" "gexf" "graphml")
for graph in "${graphs[@]}"; do
    for format in "${formats[@]}"; do
        full_name=$graph.$format;
        full_name_txt=$graph.txt
        if [ -e $full_name ] && ! [ -e $full_name_txt ]
        then
            echo $full_name;
            ../augmental/scripts/to_txt.py $full_name
            if [ $? == 0 ]
            then
                break
            fi
        fi
    done;
done