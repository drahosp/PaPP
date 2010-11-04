#ifndef FRACTAL_H_INCLUDED
#define FRACTAL_H_INCLUDED

#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#define TEX_SIZE 512

double zoom;

struct pixel {
    BYTE r;
    BYTE g;
    BYTE b;
} image[TEX_SIZE*TEX_SIZE];

void GenerateImage();

#endif // FRACTAL_H_INCLUDED
