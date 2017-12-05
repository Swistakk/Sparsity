for var in "$@"; do
if [ $var != $1 ]
then
    cd $var; ../$1; cd ..;
fi
done