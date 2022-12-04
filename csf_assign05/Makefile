# Assignment 5 Makefile
# You should only need to change this if you either
# add new source files, or remove source files provided
# in the skeleton project

CXX = g++
CXXFLAGS = -g -Wall -std=c++14 -D_POSIX_C_SOURCE=200809L
CC = gcc
CFLAGS = -g -Wall -std=c11 -D_POSIX_C_SOURCE=200809L

# C++ source/object files used only for the server
CXX_SERVER_SRCS = server.cpp server_main.cpp message_queue.cpp room.cpp
CXX_SERVER_OBJS = $(CXX_SERVER_SRCS:.cpp=.o)

# C++ source/object files used only for the receiver
CXX_RECEIVER_SRCS = receiver.cpp
CXX_RECEIVER_OBJS = $(CXX_RECEIVER_SRCS:.cpp=.o)

# C++ source/object files used only for the sender
CXX_SENDER_SRCS = sender.cpp
CXX_SENDER_OBJS = $(CXX_SENDER_SRCS:.cpp=.o)

# Common C++ source/object files used by both server
# and clients
CXX_COMMON_SRCS = connection.cpp
CXX_COMMON_OBJS = $(CXX_COMMON_SRCS:.cpp=.o)

# Common C++ source/object files used only by the clients
CXX_CLIENT_SRCS = client_util.cpp
CXX_CLIENT_OBJS = $(CXX_CLIENT_SRCS:.cpp=.o)

CXX_SRCS = $(CXX_SERVER_SRCS) $(CXX_RECEIVER_SRCS) $(CXX_SENDER_SRCS) \
	$(CXX_CLIENT_SRCS)

# C source/object file (this is also common to all executables)
C_COMMON_SRCS = csapp.c
C_COMMON_OBJS = $(C_COMMON_SRCS:.c=.o)

EXES = server sender receiver

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

all : $(EXES)

server : $(CXX_SERVER_OBJS) $(CXX_COMMON_OBJS) $(C_COMMON_OBJS)
	$(CXX) -o $@ $(CXX_SERVER_OBJS) $(CXX_COMMON_OBJS) $(C_COMMON_OBJS) -lpthread

sender : $(CXX_SENDER_OBJS) $(CXX_COMMON_OBJS) $(CXX_CLIENT_OBJS) $(C_COMMON_OBJS)
	$(CXX) -o $@ \
		$(CXX_SENDER_OBJS) $(CXX_COMMON_OBJS) $(CXX_CLIENT_OBJS) $(C_COMMON_OBJS) \
		-lpthread

receiver : $(CXX_RECEIVER_OBJS) $(CXX_COMMON_OBJS) $(CXX_CLIENT_OBJS) $(C_COMMON_OBJS)
	$(CXX) -o $@ \
		$(CXX_RECEIVER_OBJS) $(CXX_COMMON_OBJS) $(CXX_CLIENT_OBJS) $(C_COMMON_OBJS) \
		-lpthread

.PHONY: solution.zip
solution.zip :
	rm -f $@
	zip -9r $@ Makefile *.cpp *.c *.h README.txt

clean :
	rm -f *.o depend.mak
	rm -f $(EXES)

depend :
	$(CXX) $(CXXFLAGS) -M $(CXX_SRCS) > depend.mak
	$(CC) $(CFLAGS) -M $(C_COMMON_SRCS) >> depend.mak

depend.mak :
	touch $@

include depend.mak
