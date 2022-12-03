.PHONY : all

CXX_OPTIONS = -g -O0

SOURCES = $(wildcard *.cpp)
PROGRAMS = $(subst .cpp,.out,$(SOURCES))

all : $(PROGRAMS)
%.out : %.cpp
	g++ $(CXX_OPTIONS) -o $@ $<
