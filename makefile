CC = g++
FLAGS = -c

LIBS = main.o vector.o 
PROG = raytracer


all: $(LIBS)
			$(CC) -o $(PROG) $(LIBS) 

vector.o: vector.h vector.cpp
					$(CC) $(FLAGS) vector.cpp vector.h
main.o: main.cpp
				$(CC) $(FLAGS) main.cpp 






#factorial.o: factorial.cpp
#	g++ -c factorial.cpp

clean:
	rm -rf *o *.bmp $(PROG)