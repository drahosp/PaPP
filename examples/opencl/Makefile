#
# Makefile for OpenCL demo programs
# Requires GLUT/OpenGL for gradient demo
#
# "make program" to make one program
# "make" or "make all" to make all executables
# "make clean" to remove executables
#

CC		= gcc
CFLAGS	= -O3 -Wall -fopenmp -lm
UNAME := $(shell uname -s)

ALL =   cl_vector cl_gl_gradient

all:  $(ALL)

%: %.c 
	$(CC) -o $@ $(CFLAGS) $< $(CLFLAGS) $(GLFLAGS)

# Windows
CLFLAGS	= -lOpenCL
GLFLAGS	= -lFreeGLUT -lOpenGL32 -lglu32
clean:
	-del *.exe

# Linux
ifeq ($(UNAME),Linux)
CLFLAGS = -lCL
GLFLAGS = -lGL -lGLU -lglut
clean:
	-rm $(ALL)
endif

# OSX
ifeq ($(UNAME),Darwin)
CLFLAGS = -framework OpenCL
GLFLAGS = -lm -framework GLUT -framework OpenGL
clean:
	-rm $(ALL)
endif
