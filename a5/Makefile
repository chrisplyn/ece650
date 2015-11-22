CC := g++
CFLAGS = -o3 -Wall -g -DDEBUG
EXEC = a5-ece650
OBJECTS = main.o list.o satcnf.o approx_1.o approx_2.o adjacencyList.o

all: $(EXEC) 

${EXEC}: ${OBJECTS}
	${CC} ${CFLAGS} -o ${EXEC} ${OBJECTS} -L. -lsat -lpthread -lrt

.PHONY: clean

clean:
	@rm *.o ${EXEC}