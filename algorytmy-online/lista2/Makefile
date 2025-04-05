CC := g++
CCFLAGS := -Wall -Wextra -pedantic -std=c++11 -O3

all: main

main: main.o cache.o distribution.o
	$(CC) $(CCFLAGS) -o main main.o cache.o distribution.o

cache.o: cache.cpp cache.hpp
	$(CC) $(CCFLAGS) -c cache.cpp

distribution.o: distribution.cpp distribution.hpp
	$(CC) $(CCFLAGS) -c distribution.cpp

main.o: main.cpp
	$(CC) $(CCFLAGS) -c main.cpp

clean:
	rm -f *.o
