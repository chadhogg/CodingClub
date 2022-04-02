.PHONY : all clean

CXX_OPTIONS = -g

all : EulersNumber.out

%.out : %.cpp
	g++ $(CXX_OPTIONS) -o $@ $<


clean :
	rm *~ EulersNumber.out
