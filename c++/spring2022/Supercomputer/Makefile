.PHONY : all clean

CXX_OPTIONS = -g -O3

all : Supercomputer.out

%.out : %.cpp
	g++ $(CXX_OPTIONS) -o $@ $<


clean :
	rm *~ Supercomputer.out
