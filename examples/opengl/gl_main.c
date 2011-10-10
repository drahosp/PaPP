/*
 OpenGL visualization skeleton for displaying bitmap images. Just provide a GenerateImage function.
 Good starting point for all image processing exercises for parallel programming.
*/

#include <stdio.h>
#include <stdlib.h>
#include "gl_main.h"

GLuint texture;

// Initialize OpenGL state
void init() {
	// Texture setup
    glEnable(GL_TEXTURE_2D);
    glGenTextures( 1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Other
    glClearColor(0,0,0,0);
    gluOrtho2D(-1,1,-1,1);
    glLoadIdentity();
    glColor3f(1,1,1);
}

// Generate and display the image.
void display() {
    // Call user image generation
    GenerateImage();
    // Copy image to texture memory
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_SIZE, TEX_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // Clear screen buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // Render a quad
    glBegin(GL_QUADS);
        glTexCoord2f(1,0); glVertex2f(1,-1);
        glTexCoord2f(1,1); glVertex2f(1,1);
        glTexCoord2f(0,1); glVertex2f(-1,1);
        glTexCoord2f(0,0); glVertex2f(-1,-1);
    glEnd();
    // Display result
    glFlush();
    glutPostRedisplay();
    glutSwapBuffers();
}

// Main entry function
int main(int argc, char ** argv) {
    // Init GLUT
    glutInit(&argc, argv);
    glutInitWindowSize(TEX_SIZE, TEX_SIZE);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow("Fractal");
    // Set up OpenGL state
    init();
    // Run the control loop
    glutDisplayFunc(display);
    glutMainLoop();
    return EXIT_SUCCESS;
}
