CC = g++
CFLAGS = 
LDFLAGS = 
LIBS = Light.o Color.o Camera.o Vector.o Ray.o main.o
HEADERS = Light.h Color.h Vector.h Ray.h Camera.h 
PROG = raytracer

all: $(LIBS) 
			${CC} ${CFLAGS} $(LIBS) $(LDFLAGS) -o $(PROG)

main.o: main.cpp $(HEADERS)
				 ${CC} ${CFLAGS} -c main.cpp

Vector.o:  Vector.cpp Vector.h
						${CC} ${CFLAGS} -c Vector.cpp

Ray.o: Ray.cpp Ray.h Vector.h 
						${CC} ${CFLAGS} -c Ray.cpp

Camera.o:  Vector.o Camera.cpp Camera.h 
						${CC} ${CFLAGS} -c Camera.cpp

Color.o:  Color.cpp Color.h 
						${CC} ${CFLAGS} -c Color.cpp

Light.o:  Vector.o Color.o Light.cpp Light.h 
						${CC} ${CFLAGS} -c Light.cpp

clean:
	rm -rf *o *.bmp $(PROG)