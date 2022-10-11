CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: csim_driver.o Csim.o Cache.o Set.o
	g++ -o csim csim_driver.o Csim.o Cache.o

csim_driver.o: csim_driver.cpp Csim.h Cache.h
	g++ -c csim_driver.cpp $(CFLAGS)

Csim.o: Csim.cpp
	g++ -c Csim.cpp $(CFLAGS)

Cache.o: Cache.cpp Set.h
	g++ -c Cache.cpp $(CFLAGS)

Set.o: Set.cpp
	g++ -c Set.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o csim *~ *.gch
