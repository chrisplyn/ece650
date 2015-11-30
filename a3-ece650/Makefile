CXX = gcc
CFLAGS = -O3 -Wall -g
A2 = a2-ece650
A3 = a3-ece650
RGEN = rgen
OBJECTS = main.o list.o queue.o adjacencyList.o
RGENOBJ = rgen.o adjacencyList.o list.o

all: $(A2) $(A3) $(RGEN)

${A2}: ${OBJECTS}
	${CXX} ${CXXFLAGS} -o ${A2} ${OBJECTS}

${A3}: pipe.c
	${CXX} ${CXXFLAGS} pipe.c -o ${A3}

${RGEN}: ${RGENOBJ}
	${CXX} ${CXXFLAGS} -o ${RGEN} ${RGENOBJ}

.PHONY: clean

clean:
	@rm *.o $(A2) $(A3) $(RGEN)