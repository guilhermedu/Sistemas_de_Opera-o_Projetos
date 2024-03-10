CXX = g++
CXXFLAGS = -Wall -Wfatal-errors -ggdb -pthread

TARGETS = questao2

.PHONY:	all clean cleanall

all: $(TARGETS)

questao2: questao2.o  process.o utils.o
	$(CXX) -o $@ $^ $(CXXFLAGS)
clean:
	rm -f *.o

cleanall:	clean
	rm -f $(TARGETS) 
