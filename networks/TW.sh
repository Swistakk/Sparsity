single_timeout=300
for input in *.txtg; do
    #name=${input:0:-5};
    #diff -bsq orders/$name.bywreach$rad.txt orders/$name.by2wreach$rad.txt 
    #rm orders/$name.by2wreach$rad.txt
    /usr/bin/time -q -f "%E" timeout $single_timeout ~/Dokumenty/Mgr/OldRepo/dom/TW --in=$input;
done
    