CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic

INC=include
SRC=src
CPPFLAG=-I$(INC)

ROOTFLAG=`root-config --cflags --libs`

all: runPlotter

test: $(SRC)/test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(CPPFLAG)

plotter.o: $(SRC)/plotter.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CPPFLAG) $(ROOTFLAG)

runPlotter: $(SRC)/runPlotter.cpp plotter.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CPPFLAG) $(ROOTFLAG)

.PHONY:
	clean
	all

clean:
	rm -f test
	rm -f runPlotter
