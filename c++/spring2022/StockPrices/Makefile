.PHONY : all clean

CXX_OPTIONS = -g

all : StockPrices.out

%.out : %.cpp
	g++ $(CXX_OPTIONS) -o $@ $<


clean :
	rm *~ StockPrices.out
