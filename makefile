#AUTHOR: Miguel Correlo
#EMAIL: correlomm at gmail com
CC = g++
CFLAGS =
LDFLAGS =
LIBS = Loader.o Util.o pugixml.o Source.o Plane.o Triangle.o Object.o Sphere.o Light.o Color.o Camera.o Vector.o Ray.o main.o
HEADERS = Loader.h Structs.h Util.h $(LIBPATH)pugiconfig.hpp $(LIBPATH)pugixml.hpp Source.h Plane.h Triangle.h Object.h Sphere.h Light.h Color.h Vector.h Ray.h Camera.h
PROG = raytracer
LIBPATH = libs/

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

Light.o: Source.h Vector.o Color.o Light.cpp Light.h
						${CC} ${CFLAGS} -c Light.cpp

Sphere.o:  Object.o Vector.o Color.o Object.h Sphere.cpp Sphere.h
						${CC} ${CFLAGS} -c Sphere.cpp

Object.o:  Ray.h Color.h Vector.h Object.cpp Object.h
						${CC} ${CFLAGS} -c Object.cpp

Plane.o:  Object.h Color.h Vector.h Plane.cpp Plane.h
						${CC} ${CFLAGS} -c Plane.cpp

Source.o: Source.cpp Source.h
			${CC} ${CFLAGS} -c Source.cpp

Util.o: Structs.h Util.cpp Util.h
			${CC} ${CFLAGS} -c Util.cpp

Triangle.o: Object.o Vector.o Color.o Object.h Triangle.cpp Triangle.h
						${CC} ${CFLAGS} -c Triangle.cpp

Loader.o: Object.o Vector.o Color.o Object.h Loader.cpp Loader.h
						${CC} ${CFLAGS} -c Loader.cpp

pugixml.o: $(LIBPATH)pugiconfig.hpp $(LIBPATH)pugixml.hpp $(LIBPATH)pugixml.cpp
			${CC} ${CFLAGS} -c $(LIBPATH)pugixml.cpp

clean:
	rm -rf *o scene.bmp
