
PROGS := SpatialDistortion
CPPSTD = -std=c++11
LIBS = -lboost_system -lboost_filesystem

all: $(PROGS)

SpatialDistortion: gameengine.o space.o commands.o main.o
	g++ ${CPPSTD} -g gameengine.o space.o commands.o main.o -o SpatialDistortion ${LIBS}

main.o: main.cpp
	g++ ${CPPSTD} -c main.cpp
	
gameengine.o: gameengine.cpp gameengine.hpp
	g++ ${CPPSTD} -c gameengine.cpp
	
space.o: space.cpp space.hpp
	g++ ${CPPSTD} -c space.cpp

commands.o: Commands/commands.cpp Commands/commands.hpp
	g++ ${CPPSTD} -c Commands/commands.cpp

clean:
	rm -f $(PROGS) *.o *~
