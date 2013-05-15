CC = g++
CFLAGS = -g
LDFLAGS = -lm
LIBS = Vector.o main.o
HEADERS = Vector.h

all: $(LIBS) 
			${CC} ${CFLAGS} $(LIBS) $(LDFLAGS) -o raytracer

main.o: main.cpp $(HEADERS)
				 ${CC} ${CFLAGS} -c main.cpp

Vector.o:  Vector.cpp Vector.h
						${CC} ${CFLAGS} -c Vector.cpp



clean:
	rm -rf *o *.bmp $(PROG)