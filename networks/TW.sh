for input in *.txtg; do
    #name=${input:0:-5};
    #diff -bsq orders/$name.bywreach$rad.txt orders/$name.by2wreach$rad.txt 
    #rm orders/$name.by2wreach$rad.txt
    ~/Dokumenty/Mgr/OldRepo/dom/TW --in=$input;
done
    