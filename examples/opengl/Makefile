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

ALL =   gl_gradient gl_projection gl_transformation gl_vbo gl_light

all:  $(ALL)

%: %.c
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

# Windows
LFLAGS	= -lm -lglew32 -lFreeGLUT -lOpenGL32 -lglu32
clean:
	-del *.exe

# Linux
ifeq ($(UNAME),Linux)
LFLAGS = -lm -lGLEW -lGL -lGLU -lglut
clean:
	-rm $(ALL)
endif

# OSX
ifeq ($(UNAME),Darwin)
LFLAGS = -lm -framework GLUT -framework OpenGL
clean:
	-rm $(ALL)
endif
