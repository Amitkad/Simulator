CC_FLAGS = -std=c++11 -g -Wall -O2 -ldl
SO_FLAGS = -std=c++11 -shared -fPIC -o
# may add boost libraries here
LD_FLAGS = -lm

all: simulator 201191897_A_.so 201191897_B_.so 201191897_C_.so

simulator:  main.o importFiles.o House.o SimpleSensor.o Simulator.o 
	g++ -o $@ $^ $(CC_FLAGS)

main.o: main.cpp importFiles.cpp SimpleSensor.cpp  House.cpp Simulator.cpp 
	g++ -c  $(CC_FLAGS) main.cpp

importFiles.o: importFiles.cpp importFiles.h
	g++ -c  $(CC_FLAGS) importFiles.cpp

House.o: House.cpp House.h
	g++ -c  $(CC_FLAGS) House.cpp
	
SimpleSensor.o: SimpleSensor.cpp SimpleSensor.h
	g++ -c   $(CC_FLAGS) SimpleSensor.cpp
	
Simulator.o: Simulator.cpp Simulator.h
	g++ -c   $(CC_FLAGS) Simulator.cpp 

201191897_A_.so: ../algSRC/201191897_A_.cpp ../algSRC/201191897_A_.h
	g++  $(SO_FLAGS) ../algSO/201191897_A_.so ../algSRC/201191897_A_.cpp
	
201191897_B_.so: ../algSRC/201191897_B_.cpp ../algSRC/201191897_B_.h
	g++  $(SO_FLAGS) ../algSO/201191897_B_.so ../algSRC/201191897_B_.cpp
	
201191897_C_.so: ../algSRC/201191897_C_.cpp ../algSRC/201191897_C_.h
	g++  $(SO_FLAGS) ../algSO/201191897_C_.so ../algSRC/201191897_C_.cpp
	
	
clean:
	rm -f Simulator.o SimpleSensor.o  House.o  main.o simulator ../algSO/201191897_A_.so ../algSO/201191897_B_.so ../algSO/201191897_C_.so
