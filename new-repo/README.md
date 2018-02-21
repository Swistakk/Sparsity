This file is to provide a short walkthrough on functionalities of provided
algorithms and their usages.

Graph format we use is simply list of edges.
Each line should have exactly two identifiers which should be
tab-separated strings (however most of provided programs
will work even if identifiers are space-separated).
We use .txtg extensions to denote such format.

Orders we work with are in a format of space-separated identifiers of vertices
printed in one line. We use .txt extension.

################ WEAK COLORING NUMBERS ################

Before using any programs executing "make" once in "src" directory is required.
Usages of most programs (except treedepth, treewidth and dtf) are explained 
when they are tried to be used. For program X, you can execute "./X --h"
in order to get verbose explanation of how it should be used.

Unfortunately there are some inconsistencies between how programs output
computed order. Some programs will print their order on stdout,
some of them will put it in a file with prescribed name in subdirectory
"orders" of directory where input graph resides (that needs to be created beforehand).
However in the second case such programs will also support
optional flag --o that allows to specify output file.

For the ease of exposure let us assume that we want to execute some of provided algorithms
on graph G.txtg, radius R and, if program needs it, on order O.txt.

####### WcolStats #######
WcolStats - this is a program that takes graph, order an radius as an input
and outputs some statistics about WReach_R sets. Based on --mode flag it can be either
wcol_R (size of biggest WReach_R set) of that order or sizes of all WReach_R sets
or all WReach_R sets fully enumerated. Prints to stdout.


####### Main approaches producing orders #######
Sortdeg - this is a simple heuristic that takes graph and radius (R) as an order
and produces and order of vertices sorting nonincreasingly by sizes of
their r-neighbourhoods (where R=1 is main usage).
Sorting by degree turned out to achieve
best results on real-world graphs even though it doesn't have any theoretical guarantees
and can be easily fooled by artificial examples.
Even if you want to approximate wcol_R with R>1, you should
use this program with R=1. This program also supports larger values of R
but they don't produce reasonable orderings and should not be used.
It puts produced order into file orders/G.sortdegR.txt

FlatWcol - this is a program implementing algorithm with flat decompositions.
It takes graph as input only, but since many variants of it were
implemented it also needs to be supplied with few execution flags
corresponding to rules of ordering vertices within blobs
and choice of new vertex. They are explained in details
in usage explanation.
Recommended choice of these flags is:
--order=sortdeg --rev=no --rule=all
This program puts produced order into file orders/G.flat.xyz.txt
where xyz depends on choice of discussed flags.
x = (b if --order=bfs, d if --order=dfs, s if --order=sortdeg)
y = (n if --rev=no, y if --rev=yes)
z = (a if --rule=all, o if --rule=neis_of_past, i if --rule=neis_in_past)

treedepth - before using this program please go to "src/td-heuristic" subdirectory
and call "make" once. This code is written in C, but we provided wrapper code
in Python to execute it. It prints produced orer to stdout.
Usage: python3.5 t.py G.txtg


treewidth - this heuristic has its own directory with its internal README,
so please refer to this.

dtf - this is an algorithm implementing distance constrained transitive fraternal augmentation.
It takes graph and radius as an input. It prints produced order to stdout.
Usage: augmental/scripts.dtf.py G.txtg R

####### Other heuristics #######
Degeneracy
RandomWcol


####### Local Search #######
LSWcolBoth
LSWcolOld
LSWcolNew

####### Misc #######
FixIsolatedInOrder - some programs handle incorrectly vertices v such that
there is an edge "vv" (loop), but there are no other edges that are adjacent
to this vertex. Use this program to 
Degeneracy



