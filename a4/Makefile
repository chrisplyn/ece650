CC := g++
CXXFLAGS = -O3 -g -Wall
EXEC = a4-ece650
OBJECTS = main.o list.o

all: $(EXEC) 

${EXEC}: ${OBJECTS}
	${CC} ${CXXFLAGS} -o ${EXEC} ${OBJECTS} -L. -lsat

.PHONY: clean

clean:
	@rm *.o ${EXEC}