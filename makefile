CXX := $(CXX) -std=c++11
insundtree = -g -Wall -fPIC



A4: a_stern.o maze.o unit.o
	$(CXX) $(insundtree) -o $@ $^

a_stern.o: a_stern.cpp maze.h unit.h
	$(CXX) $(insundtree) -c $<
	
maze.o: maze.cpp unit.h maze.h
	$(CXX) $(insundtree) -c $<

.PHONY: clean

clean:
	rm -f A4 a_stern.o maze.o