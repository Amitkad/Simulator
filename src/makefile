CC_FLAGS = -std=c++11 -g -Wall -O2 -pedantic -pthread

# may add boost libraries here
LD_FLAGS = -lm

all: simulator

simulator:  main.o auxillary.o configParser.o  House.o randAlgorithm.o  SimpleSensor.o Simulator.o 
	g++ -o $@ $^ $(CC_FLAGS)

main.o: main.cpp randAlgorithm.cpp SimpleSensor.cpp auxillary.cpp configParser.cpp House.cpp Simulator.cpp 
	g++ -c  $(CC_FLAGS) main.cpp
	
auxillary.o: auxillary.cpp auxillary.h
	g++ -c  $(CC_FLAGS) auxillary.cpp

configParser.o: configParser.cpp configParser.h
	g++ -c  $(CC_FLAGS) configParser.cpp

House.o: House.cpp House.h
	g++ -c  $(CC_FLAGS) House.cpp
	
randAlgorithm.o: randAlgorithm.cpp randAlgorithm.h
	g++ -c  $(CC_FLAGS) randAlgorithm.cpp
	
SimpleSensor.o: SimpleSensor.cpp SimpleSensor.h
	g++ -c   $(CC_FLAGS) SimpleSensor.cpp
	
Simulator.o: Simulator.cpp Simulator.h
	g++ -c   $(CC_FLAGS) Simulator.cpp 


clean:
	rm -f Simulator.o SimpleSensor.o randAlgorithm.o House.o configParser.o auxillary.o main.o simulator
