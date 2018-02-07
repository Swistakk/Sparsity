# Sparsity

Most important codes for wcol:
SortDeg.cpp
LSWcolBoth.cpp
WcolStats.cpp
These files are required and sufficient (with their dependencies) to generate
orders by best possible approach which is sortdeg1+local search.

Scripts for wcol:

{random_wcol, degeneracy, sortdeg1, sortdegR, td, fill-in, felix, berlin_wcol, christoph}.sh
Execute following code on all graphs in a current directory

Let's call sortdeg1, td, fill-in, berlin.sna and (felix with appropriate radius) as _main_ approaches.

LS.sh - executes local search on orders produced by main approaches on all graphs in current directory.
This script has a variable TIMEOUT that bounds time of its execution (in seconds)
and can be arbitrarily changed. It can be also -1 if you want to not bound it
and allow it to run as long as it thinks there is nonnegligible chance to improve.

Since in this repo graphs are partitioned into small/medium/big/huge directories
there is also script execute.sh that takes a script and list of directories
and executes this script in these directories.
For example "./execute.sh sortdeg1.sh small big"
If some script has not typical usage (different than what I have just described)
then it is usually put in comment in its first line

generate_csv_wcol.sh - assumes that orders of interest are already generated
and produced csv where rows are indexed by (test x radius) and columns are indexed by algorithm

Steps to obtain best wcol numbers:

Note that currently paths to binary files are often either of type "~/Dokumenty/Mgr/dom/SortDeg"
or "../../dom/LSWcolBoth". First one is of course bad and second is also bad because
it assumes some particular depth in the directory tree about the place where it is executed.
I am aware that they should be corrected to something like $ROOT/dom/SortDeg,
but this is not done so far, so if you want to use my scripts
you need to adjust paths accordingly.

If our goal is to produce report with wcol numbers of best orders you need to:
0) Adjust paths in scripts which we will use
1) Put our graphs in subdirectory (call it "graphs")
2) Execute "./execute.sh sortdeg1.sh graphs"
3) Change "progs="sortdeg1 berlin.sna td fill-in "$felix"" to "progs="sortdeg1"" in LS.sh
4) Execute "./execute.sh LS.sh graphs"
5) Change
progs="random deg1 deg2 deg3 deg4 deg5 sortdeg1 sortdeg2 sortdeg3 sortdeg4 sortdeg5 td fill-in felix1 felix2 felix3 felix4 felix5 $berlins christoph";
last_prog="christoph"
into
progs="sortdeg1"
last_prog="sortdeg1"
in generate_csv_wcol.sh
6) Execute "(./execute.sh ./generate_csv_wcol.sh graphs) > graphs_rep.csv"


Procedure I have written above is not tested, it should be correct, but I do not guarantee it.
If you encounter any problems please let me know by an e-mail.


