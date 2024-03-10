CXX = g++
CXXFLAGS = -Wall -Wfatal-errors -ggdb -pthread

TARGETS = questao3

.PHONY:	all clean cleanall

all: $(TARGETS)

questao3: questao3.o  thread.o utils.o
	$(CXX) -o $@ $^ $(CXXFLAGS)
clean:
	rm -f *.o

cleanall:	clean
	rm -f $(TARGETS) 
