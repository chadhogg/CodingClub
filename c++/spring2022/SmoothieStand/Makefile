.PHONY : all clean

CXX_OPTIONS = -g

all : SmoothieStand.out

%.out : %.cpp
	g++ $(CXX_OPTIONS) -o $@ $<


clean :
	rm *~ StringMatching.out
