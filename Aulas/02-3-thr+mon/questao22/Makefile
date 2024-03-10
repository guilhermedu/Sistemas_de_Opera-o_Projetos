CXX = g++
CXXFLAGS = -Wall -Wfatal-errors -ggdb -pthread

TARGETS = main

.PHONY:	all clean cleanall

all: $(TARGETS)

bb-unsafe: main.o  thread.o utils.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

bb-safe: main.o  thread.o utils.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f *.o

cleanall:	clean
	rm -f $(TARGETS) 
