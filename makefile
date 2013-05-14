CC = g++
FLAGS = -c

LIBS = main.o
PROG = raytracer


hello: $(LIBS)
	g++ $(LIBS) -o $(PROG)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 






#factorial.o: factorial.cpp
#	g++ -c factorial.cpp

clean:
	rm -rf *o hello