CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: csim_driver.o csim_functions.o Cache.o
	g++ -o csim csim_driver.o csim_functions.o Cache.o

csim_driver.o: csim_driver.cpp csim_functions.h Cache.h
	g++ -c csim_driver.cpp $(CFLAGS)

csim_functions.o: csim_functions.cpp
	g++ -c csim_functions.cpp $(CFLAGS)

Cache.o: Cache.cpp
	g++ -c Cache.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o csim *~ *.gch
