.PHONY : all clean

CXX_OPTIONS = -g

all : ShortestPath1.out Generator.out ShortestPath1V2.out

%.out : %.cpp
	g++ $(CXX_OPTIONS) -o $@ $<


clean :
	rm *~ ShortestPath1.out Generator.out ShortestPath1V2.out
