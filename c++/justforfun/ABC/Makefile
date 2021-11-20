.PHONY : all clean

CXX_OPTIONS = -O2
PROGRAMS = ABC.out

all : $(PROGRAMS)

%.out : %.cpp
	g++ $(CXX_OPTIONS) -o $@ $<


clean :
	rm -f *~ $(PROGRAMS)
