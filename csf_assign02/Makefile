# Makefile for CSF Assignment 2
# You should not need to modify this

CC = gcc
CFLAGS = -g -Wall -std=gnu11 -no-pie

ASMFLAGS = -g -no-pie

LDFLAGS = -no-pie

# C source files that are used in all versions of the executable
COMMON_C_SRCS = pnglite.c image.c
COMMON_C_OBJS = $(COMMON_C_SRCS:.c=.o)

# C implementation of drawing functions
C_SRCS = c_drawing_funcs.c
C_OBJS = $(C_SRCS:.c=.o)

# Assembly implementation of drawing functions
ASM_SRCS = asm_drawing_funcs.S
ASM_OBJS = $(ASM_SRCS:.S=.o)

# Source module with main() function for reading an input file
# and using the drawing functions to generate an output image
DRIVER_SRCS = c_driver.c
DRIVER_OBJS = $(DRIVER_SRCS:.c=.o)

# Source modules needed for the unit test program
TEST_SRCS = test_drawing_funcs.c tctest.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

EXES = c_draw c_test_drawing_funcs asm_draw asm_test_drawing_funcs

%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

%.o : %.S
	$(CC) $(ASMFLAGS) -c $*.S -o $*.o

all : $(EXES)

c_draw : $(DRIVER_OBJS) $(COMMON_C_OBJS) $(C_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(DRIVER_OBJS) $(COMMON_C_OBJS) $(C_OBJS) -lz

c_test_drawing_funcs : $(TEST_OBJS) $(C_OBJS) $(COMMON_C_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(TEST_OBJS) $(C_OBJS) $(COMMON_C_OBJS) -lz

asm_draw : $(DRIVER_OBJS) $(COMMON_C_OBJS) $(ASM_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(DRIVER_OBJS) $(COMMON_C_OBJS) $(ASM_OBJS) -lz

asm_test_drawing_funcs : $(TEST_OBJS) $(ASM_OBJS) $(COMMON_C_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(TEST_OBJS) $(ASM_OBJS) $(COMMON_C_OBJS) -lz


clean :
	rm -f *.o $(EXES)

depend.mak :
	touch $@

depend :
	$(CC) $(CFLAGS) -M \
		$(COMMON_C_SRCS) $(C_SRCS) $(DRIVER_SRCS) $(TEST_SRCS) \
		> depend.mak

include depend.mak
