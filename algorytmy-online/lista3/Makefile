CC := g++
CCFLAGS := -Wall -Wextra -pedantic -std=c++11 -O3

all: main

main: main.o bins.o distribution.o
	$(CC) $(CCFLAGS) -o main main.o bins.o distribution.o

bins.o: bins.cpp bins.hpp
	$(CC) $(CCFLAGS) -c bins.cpp

distribution.o: distribution.cpp distribution.hpp
	$(CC) $(CCFLAGS) -c distribution.cpp

main.o: main.cpp
	$(CC) $(CCFLAGS) -c main.cpp

clean:
	rm -f *.o
