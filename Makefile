all: project

project: main.o
	g++ main.o -o project

main.o: main.cpp image.h
	g++ -c main.cpp -o main.o

clean:
	rm -rf *.o
