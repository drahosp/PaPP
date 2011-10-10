#ifndef FRACTAL_H_INCLUDED
#define FRACTAL_H_INCLUDED

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
typedef unsigned char BYTE;
#else
	#include <GL/freeglut.h>
	#include <GL/freeglut_ext.h>
#endif

#define TEX_SIZE 512

struct pixel {
    BYTE r;
    BYTE g;
    BYTE b;
} image[TEX_SIZE*TEX_SIZE];

void GenerateImage();

#endif // FRACTAL_H_INCLUDED