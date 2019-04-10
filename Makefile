CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic

INC=include
SRC=src
CPPFLAG=-I$(INC)

ROOTFLAG=`root-config --cflags --libs`

all: test TH1Plotter

test: $(SRC)/test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(CPPFLAG)

plotter.o: $(SRC)/plotter.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CPPFLAG) $(ROOTFLAG)

TH1Plotter: $(SRC)/TH1Plotter.cpp plotter.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CPPFLAG) $(ROOTFLAG)

.PHONY:
	clean
	all

clean:
	rm -f test
	rm -f TH1Plotter
