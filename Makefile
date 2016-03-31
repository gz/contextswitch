SRC = measureSingle.c measureSwitch.c
OBJ = $(addsuffix .o, $(basename $(SRC)))
TARGET = $(basename $(OBJ))
CC = gcc
CCFLAGS = 

all: $(TARGET)

measureSingle: measureSingle.o util.o
	${CC} ${CCFLAGS} -o $@ measureSingle.o util.o

measureSwitch: measureSwitch.o util.o
	${CC} ${CCFLAGS} -o $@ measureSwitch.o util.o

util.o: util.c util.h
	${CC} ${CCFLAGS} -c $<

.c.o:
	${CC} ${CCFLAGS} -c $<

clean:
	rm -f *.o $(TARGET) 
	
