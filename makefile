CXX := $(CXX)
insundtree = -std=c++11 -Wall -pedantic -Wextra -g -O3 -Ilib/SFML-2.5.1/include/ -fPIC
LIBS = -Llib/SFML-2.5.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system



A4: a_stern.o maze.o unit.o visualizer.o
	$(CXX) $(insundtree) -o $@ $^ $(LIBS)

a_stern.o: a_stern.cpp maze.h unit.h
	$(CXX) $(insundtree) -c $<

maze.o: maze.cpp unit.h maze.h
	$(CXX) $(insundtree) -c $<

visualizer.o: visualizer.cpp unit.h visualizer.h
	$(CXX) $(insundtree) -c $<

run: A4
	export LD_LIBRARY_PATH=./lib/SFML-2.5.1/lib && ./A4


run_sfml_test: sfml_test
	export LD_LIBRARY_PATH=./lib/SFML-2.5.1/lib && ./sfml_test

sfml_test: sfml_test.o
	$(CXX) $(insundtree) -o $@ $^ $(LIBS)

sfml_test.o: sfml_test.cpp
	$(CXX) $(insundtree) -c $<

.PHONY: clean

clean:
	rm -f A4 a_stern.o maze.o sfml_test.o sfml_test

