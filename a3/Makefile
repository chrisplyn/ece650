CXX = gcc
CXXFLAGS = -O3 -Wall -std=c99
A2 = a2-ece650
A3 = a3-ece650
RGEN = rgen
OBJECTS = main.o list.o queue.o adjacencyList.o

all: $(A2) $(A3) $(RGEN)

${A2}: ${OBJECTS}
	${CXX} ${CXXFLAGS} -o ${A2} ${OBJECTS}

${A3}: pipe.c
	${CXX} ${CXXFLAGS} pipe.c -o ${A3}

${RGEN}: rgen.c
	${CXX} ${CXXFLAGS} rgen.c -o ${RGEN}

.PHONY: clean

clean:
	@rm *.o $(A2) $(A3) $(RGEN)