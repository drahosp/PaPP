/*
 OpenGL visualization skeleton for displaying bitmap images. Just provide a GenerateImage function.
 Good starting point for all image processing exercises for parallel programming.

 Example use of transformations
*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/freeglut.h>
  #include <GL/freeglut_ext.h>
#endif

#define SIZE 512

typedef struct {
   unsigned char r;
   unsigned char g;
   unsigned char b;
} pixel;

pixel image[SIZE*SIZE];
GLuint texture;

float t = 0;

// Generate and display the image.
void display() {
  // Copy image to texture memory
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SIZE, SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

  // Clear screen buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // ModelView to WorldView transformation
  glMatrixMode(GL_MODELVIEW);
  // Reset transformation
  glLoadIdentity();

  // Translation
  glTranslatef(.5, .3, 0);
  //glTranslatef(sin(t), 0, 0);

  // Scale
  //glScalef(.5,.3,0);
  //glScalef(1+sin(t), 1+sin(t), 0);

  // Rotation (angle, x, y, z)
  //glRotatef(30, 0, 0, 1);
  //glRotatef(3*t, 0, 0, 1);

  /* Rotate around (-1,-1)
  glTranslatef(-1,-1,0);
  glRotatef(4*t,0,0,1);
  glTranslatef(1,1,0);
  */

  /* Scale from (1,1)
  glTranslatef(1,1,0);
  glScalef(1+sin(t),1+sin(t),1);
  glTranslatef(-1,-1,0);
  */

  // Render a quad
  glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex2f(-1,-1);
    glTexCoord2f(1,0); glVertex2f(1,-1);
    glTexCoord2f(1,1); glVertex2f(1,1);
    glTexCoord2f(0,1); glVertex2f(-1,1);
  glEnd();
  // Display result
  glFlush();
  //glutPostRedisplay();
  glutSwapBuffers();
}

// Handle keyboard input
void keyboard(unsigned char key, int x, int y) {
  switch(key) {
    case 'q':
      exit(0);
      break;
  }
  glutPostRedisplay();
}

// Initialize OpenGL state
void init() {
	// Texture setup
  glEnable(GL_TEXTURE_2D);
  glGenTextures( 1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Set up view projection
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(-1,1,1,-1);

  // Other
  glClearColor(0,0,1,0);
  glColor3f(1,1,1);

  // Load Image
  FILE *f;
  if ( (f=fopen("image.rgb","rb")) == NULL ) {
    printf("Error loading file!\n");
    exit(0);
  }
  fread(image, sizeof(image), 1, f);
  fclose(f);
}

// "Animate" t and redisplay
void update() {
  t += 0.01;
  glutPostRedisplay();
}

// Main entry function
int main(int argc, char ** argv) {
  // Init GLUT
  glutInit(&argc, argv);
  glutInitWindowSize(SIZE, SIZE);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
  glutCreateWindow("OpenGL Window");
  // Set up OpenGL state
  init();

  // Run the control loop
  glutDisplayFunc(display);
  glutIdleFunc(update);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return EXIT_SUCCESS;
}
