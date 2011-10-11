#ifndef FRACTAL_H_INCLUDED
#define FRACTAL_H_INCLUDED

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#else
	#include <GL/freeglut.h>
	#include <GL/freeglut_ext.h>
#endif

#define TEX_SIZE 512

struct pixel {
    GLbyte r;
    GLbyte g;
    GLbyte b;
} image[TEX_SIZE*TEX_SIZE];

void GenerateImage();

#endif // FRACTAL_H_INCLUDED