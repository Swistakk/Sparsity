EXECS=WcolStats greedy BerlinWcol Degeneracy UQW-TGV UQW-TGV2 UQW-MFCS VerifierUQW VerifierUQWBrut GenUQWTest SortDeg LSWcolOld LSWcolNew RandomWcol ComputeWcolBrut LSWcolBoth UQWPatcher CheckIndependent UQWMix

OPTFLAGS=-O2 -std=c++11
OPTLOCALFLAGS= -DLOCAL -std=c++11 -O2
DBGFLAGS= -DLOCAL -lubsan -std=c++11 -fsanitize=address -g -fsanitize=undefined -pedantic -D_GLIBCXX_DEBUG -Wall -Wextra -Wfloat-equal -Wlogical-op -Wshadow -Wunused
#CXXFLAGS= -DLOCAL -lubsan -std=c++11 -fsanitize=address -g -fsanitize=undefined -pedantic -D_GLIBCXX_DEBUG -Wall -Wextra -Wfloat-equal -Wlogical-op -Wshadow -Wunused
CXXFLAGS=-DLOCAL -std=c++11 -O2
#LDFLAGS += -fsanitize=address -fsanitize=undefined
SOURCES=$(wildcard *.cpp)
DEPENDS=$(patsubst %.cpp,.%.depends,$(SOURCES))
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

all: $(EXECS)

WcolStats: WcolStats.o ReadTxt.o FilesOps.o ComputeWReach.o FlagParser.o
	$(CXX) $(LDFLAGS) $^ -o $@

VerifierUQW: VerifierUQW.o ReadTxt.o FilesOps.o CommonGraph.o ComputeWReach.o
	$(CXX) $(LDFLAGS) $^ -o $@	

VerifierUQWBrut: VerifierUQWBrut.o ReadTxt.o FilesOps.o
	$(CXX) $(LDFLAGS) $^ -o $@	
	
GenUQWTest: GenUQWTest.o ReadTxt.o FilesOps.o
	$(CXX) $(LDFLAGS) $^ -o $@	
	
greedy: greedy.o ReadTxt.o FilesOps.o
	$(CXX) $(LDFLAGS) $^ -o $@

BerlinWcol: BerlinWcol.o ReadTxt.o FilesOps.o FlagParser.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
Degeneracy: Degeneracy.o ReadTxt.o FilesOps.o FlagParser.o ComputeWReach.o ComputeDegeneracy.o CommonGraph.o
	$(CXX) $(LDFLAGS) $^ -o $@

UQW-TGV: UQW-TGV.o ReadTxt.o FilesOps.o ComputeWReach.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
UQW-TGV2: UQW-TGV2.o ReadTxt.o FilesOps.o ComputeWReach.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
UQW-MFCS: UQW-MFCS.o ReadTxt.o FilesOps.o ComputeWReach.o
	$(CXX) $(LDFLAGS) $^ -o $@

SortDeg: SortDeg.o ReadTxt.o FilesOps.o FlagParser.o ComputeWReach.o
	$(CXX) $(LDFLAGS) $^ -o $@

LSWcolOld: LSWcolOld.o ReadTxt.o FilesOps.o ComputeWReach.o FlagParser.o
	$(CXX) $(LDFLAGS) $^ -o $@

LSWcolNew: LSWcolNew.o ReadTxt.o FilesOps.o ComputeWReach.o FlagParser.o
	$(CXX) $(LDFLAGS) $^ -o $@

LSWcolBoth: LSWcolBoth.o ReadTxt.o FilesOps.o ComputeWReach.o FlagParser.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
RandomWcol: RandomWcol.o ReadTxt.o FilesOps.o FlagParser.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
ComputeWcolBrut: ComputeWcolBrut.o ReadTxt.o FilesOps.o FlagParser.o
	$(CXX) $(LDFLAGS) $^ -o $@

UQWPatcher: UQWPatcher.o ReadTxt.o FilesOps.o FlagParser.o ComputeWReach.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
CheckIndependent: CheckIndependent.o ReadTxt.o FilesOps.o FlagParser.o Independent.o ComputeWReach.o ComputeDegeneracy.o CommonGraph.o
	$(CXX) $(LDFLAGS) $^ -o $@

UQWMix: UQWMix.o ReadTxt.o FilesOps.o FlagParser.o ComputeWReach.o CommonGraph.o Independent.o ComputeDegeneracy.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
.%.depends: %.cpp
	$(CXX) $(OPTFLAGS) -MM $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(EXECS) $(DEPENDS)

-include $(DEPENDS)