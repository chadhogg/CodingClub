.PHONY : all

CXX_OPTIONS = -g -pg

all : ShortestPath1.out Generator.out

ShortestPath1.out : ShortestPath1.cpp
	g++ $(CXX_OPTIONS) -o $@ $<

Generator.out : Generator.cpp
	g++ $(CXX_OPTIONS) -o $@ $<
