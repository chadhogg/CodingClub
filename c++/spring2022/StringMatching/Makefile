.PHONY : all clean

CXX_OPTIONS = -g

all : StringMatching.out

%.out : %.cpp
	g++ $(CXX_OPTIONS) -o $@ $<


clean :
	rm *~ StringMatching.out
