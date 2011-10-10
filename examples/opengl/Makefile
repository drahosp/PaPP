#
# Makefile for OpenGL demo programs
#
# "make program" to make one program
# "make" or "make all" to make all executables
# "make clean" to remove executables
#

CC		= gcc
CFLAGS	= -O3 -Wall -fopenmp
UNAME := $(shell uname -s)

# Windows
LFLAGS	= -lm -lFreeGLUT -lOpenGL32 -lglu32

# Linux
ifeq ($(UNAME),Linux)
LFLAGS = -lm -lgl -lglu -lglut
endif

# OSX
ifeq ($(UNAME),Darwin)
LFLAGS = -lm -framework GLUT -framework OpenGL
endif

ALL =   gl_gradient

all:  $(ALL)

%: %.c 
	$(CC) -o $@ gl_main.c $(CFLAGS) $< $(LFLAGS)

clean:
	-rm $(ALL)
