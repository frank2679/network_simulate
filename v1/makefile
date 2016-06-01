main: main.o module.o myclass.o
	g++ --std=c++11 main.o module.o myclass.o -o main
main.o: main.cc
	g++ --std=c++11 main.cc -c
module.o: module.cc
	g++ --std=c++11 module.cc -c
myclass.o: myclass.cc
	g++ --std=c++11 myclass.cc -c
clean: 
	rm *.o
run: 
	./main 
