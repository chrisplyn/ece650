CXX = gcc
CXXFLAGS = -O3 -g -Wall -MMD -std=c99
EXEC = a2-ece650
OBJECTS = main.o list.o queue.o adjacencyList.o

all: $(EXEC) 

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} -o ${EXEC} ${OBJECTS}

.PHONY: clean

clean:
	@rm *.o ${EXEC}