/*
 OpenGL visualization skeleton for displaying bitmap images. Just provide a GenerateImage function.
 Good starting point for all image processing exercises for parallel programming.

 Example use of projections
*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/freeglut.h>
  #include <GL/freeglut_ext.h>
#endif

typedef struct {
   unsigned char r;
   unsigned char g;
   unsigned char b;
} pixel;

#define SIZE 512
pixel image[SIZE*SIZE];
GLuint texture;

float t = 0;

void printMatrix(int proj) {
  int i,j;
  GLdouble mat[4][4];
  glGetDoublev(proj,(GLdouble *) mat);
  for(i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      printf("%f\t", mat[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

// Generate and display the image.
void display() {
  // Copy image to texture memory
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SIZE, SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

  // Clear screen buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // ModelView to WorldView transformation
  glMatrixMode(GL_MODELVIEW);
  // Reset transformation
  glLoadIdentity();
  // Move camera back
  glTranslatef(0,0,-3);
  // Rotate world
  glRotatef(10*t, 0, 1, 0);

  // Render quads
  glBegin(GL_QUADS);
    glColor3f(0,1,0);
    glTexCoord2f(0,1); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(1,1,-1);
    glTexCoord2f(0,0); glVertex3f(-1,1,-1);
    glColor3f(1,0,0);
    glTexCoord2f(0,1); glVertex3f(-1,-1,1);
    glTexCoord2f(1,1); glVertex3f(1,-1,1);
    glTexCoord2f(1,0); glVertex3f(1,1,1);
    glTexCoord2f(0,0); glVertex3f(-1,1,1);
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
    case 'm':
      printMatrix(GL_MODELVIEW_MATRIX);
      break;
    case 'p':
      printMatrix(GL_PROJECTION_MATRIX);
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
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  //glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Set up view projection
  glMatrixMode(GL_PROJECTION);
  //gluOrtho2D(-1,1,-1,1);
  gluPerspective(75, 1, .1, 10);

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

  // Generate Mip Maps
  //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, SIZE, SIZE, GL_RGB, GL_UNSIGNED_BYTE, image );

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
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
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
