.PHONY : all clean

CXX_OPTIONS = -O2
PROGRAMS = 10KindsOfPeople.out Generator.out

all : $(PROGRAMS)

%.out : %.cpp
	g++ $(CXX_OPTIONS) -o $@ $<


clean :
	rm -f *~ $(PROGRAMS)
