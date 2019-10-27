
PROGS := SpatialDistortion

all: $(PROGS)

SpatialDistortion: gameengine.o space.o main.o
	g++ -g gameengine.o space.o main.o -o SpatialDistortion

main.o: main.cpp
	g++ -c main.cpp
	
gameengine.o: gameengine.cpp gameengine.hpp
	g++ -c gameengine.cpp
	
space.o: space.cpp space.hpp
	g++ -c space.cpp


clean:
	rm -f $(PROGS) *.o *~
