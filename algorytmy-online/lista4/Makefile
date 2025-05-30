CC := g++
CCFLAGS := -Wall -Wextra -pedantic -std=c++20 -O3

all: main

main: main.o distribution.o graph.o page_migration.o
	$(CC) $(CCFLAGS) -o main main.o distribution.o graph.o page_migration.o

distribution.o: distribution.cpp distribution.hpp
	$(CC) $(CCFLAGS) -c distribution.cpp

graph.o: graph.cpp graph.hpp
	$(CC) $(CCFLAGS) -c graph.cpp

page_migration.o: page_migration.cpp page_migration.hpp
	$(CC) $(CCFLAGS) -c page_migration.cpp

main.o: main.cpp
	$(CC) $(CCFLAGS) -c main.cpp

clean:
	rm -f *.o
