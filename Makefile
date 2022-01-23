CC = g++
CFLAGS = -o
PT = -lpthread

all: MatrixMultiply

MatrixMultiply: MatrixP.cpp
	${CC} ${CFLAGS} MatrixMultiply MatrixP.cpp ${PT}

.PHONY: clean
clean:
	rm -f MatrixMultiply *.o