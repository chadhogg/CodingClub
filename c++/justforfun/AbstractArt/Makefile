.PHONY : all clean

CXX_OPTIONS = -g
PROGRAMS = AbstractArt.out

all : $(PROGRAMS)

%.out : %.cpp
	g++ $(CXX_OPTIONS) -o $@ $<


clean :
	rm -f *~ $(PROGRAMS)
