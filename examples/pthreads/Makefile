#
# Makefile for POSIX threads demo programs
#
# "make program" to make one program
# "make" or "make all" to make all executables
# "make clean" to remove executables
#

CC		= gcc
CFLAGS	= -O3 -Wall
LFLAGS	= -lm -lpthread
UNAME := $(shell uname -s)

ALL =   pt_basic \
	pt_condition1 \
	pt_condition2 \
	pt_condition3 \
	pt_detached \
	pt_dotprod \
	pt_hello1 \
	pt_hello2 \
	pt_join1 \
	pt_join2 \
	pt_mutex \
	pt_semaphore

all:  $(ALL)

%: %.c 
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

# Windows
clean:
	-del *.exe

# Linux
ifeq ($(UNAME),Linux)
clean:
	-rm $(ALL)
endif

# OSX
ifeq ($(UNAME),Darwin)
clean:
	-rm $(ALL)
endif
