/*
 Example of generating bitmaps using GenerateImage and the prepared GLUT OpenGL visualization.
*/
#include "gl_main.h"

// Available variables:
// zoom - zoom level controlled by mousewheel
// image[TEX_SIZExTEX_SIZE] - image bitmap

void GenerateImage() {
    // Generate image
    int x,y;
    for (x=0;x<TEX_SIZE;x++) {
        for (y=0;y<TEX_SIZE;y++) {
            image[x+y*TEX_SIZE].r = x/2;
            image[x+y*TEX_SIZE].g = y/2;
            image[x+y*TEX_SIZE].b = 0;
        }
    }
}
