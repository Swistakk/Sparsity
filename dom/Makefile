EXECS=VerifierWcol greedy BerlinWcol Degeneracy UQWFirst UQWLast VerifierUQW VerifierUQWBrut GenUQWTest SortDeg

OPTFLAGS=-O2 -std=c++11
OPTLOCALFLAGS= -DLOCAL -std=c++11 -O2
DBGFLAGS= -DLOCAL -lubsan -std=c++11 -fsanitize=address -g -fsanitize=undefined -pedantic -D_GLIBCXX_DEBUG -Wall -Wextra -Wfloat-equal -Wlogical-op -Wshadow -Wunused
#CXXFLAGS= -DLOCAL -lubsan -std=c++11 -fsanitize=address -g -fsanitize=undefined -pedantic -D_GLIBCXX_DEBUG -Wall -Wextra -Wfloat-equal -Wlogical-op -Wshadow -Wunused
#CXXFLAGS=-DLOCAL -std=c++11 -O2
LDFLAGS += -fsanitize=address -fsanitize=undefined
SOURCES=$(wildcard *.cpp)
DEPENDS=$(patsubst %.cpp,.%.depends,$(SOURCES))
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

all: $(EXECS)

VerifierWcol: VerifierWcol.o ReadTxt.o FilesOps.o ComputeWReach.o
	$(CXX) $(LDFLAGS) $^ -o $@

VerifierUQW: VerifierUQW.o ReadTxt.o FilesOps.o
	$(CXX) $(LDFLAGS) $^ -o $@	

VerifierUQWBrut: VerifierUQWBrut.o ReadTxt.o FilesOps.o
	$(CXX) $(LDFLAGS) $^ -o $@	
	
GenUQWTest: GenUQWTest.o ReadTxt.o FilesOps.o
	$(CXX) $(LDFLAGS) $^ -o $@	
	
greedy: greedy.o ReadTxt.o FilesOps.o
	$(CXX) $(LDFLAGS) $^ -o $@

BerlinWcol: BerlinWcol.o ReadTxt.o FilesOps.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
Degeneracy: Degeneracy.o ReadTxt.o FilesOps.o
	$(CXX) $(LDFLAGS) $^ -o $@

UQWFirst: UQWFirst.o ReadTxt.o FilesOps.o ComputeWReach.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
UQWLast: UQWLast.o ReadTxt.o FilesOps.o ComputeWReach.o
	$(CXX) $(LDFLAGS) $^ -o $@

SortDeg: SortDeg.o ReadTxt.o FilesOps.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
	
.%.depends: %.cpp
	$(CXX) $(DBGFLAGS) -MM $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(EXECS) $(DEPENDS)

-include $(DEPENDS)