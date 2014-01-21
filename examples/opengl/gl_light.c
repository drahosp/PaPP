/*
 OpenGL visualization skeleton for displaying bitmap images. Just provide a GenerateImage function.
 Good starting point for all image processing exercises for parallel programming.

 Example use of lighting and materials
*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#ifdef __APPLE__
// On apple we do not need glew
  #define glewInit()
  #include <GLUT/glut.h>
#else
// On other platforms use glew for compatibility
  #include <GL/glew.h>
  #include <GL/freeglut.h>
  #include <GL/freeglut_ext.h>
#endif

typedef struct {
   unsigned char r;
   unsigned char g;
   unsigned char b;
} pixel;

#define SIZE 512
pixel image[SIZE][SIZE];
GLuint texture, vbo_vertices, vbo_texcoord;

float t = 0;

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

  // Draw object
  glutSolidTeapot(1);

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
void initGL() {
	// Texture setup
  glEnable(GL_TEXTURE_2D);
  glGenTextures( 1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Enable backface culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  // Set up view projection
  glMatrixMode(GL_PROJECTION);
  gluPerspective(75, 1, .1, 10);

  // Other
  glClearColor(0,0,1,0);
  glColor3f(1,1,1);
}

// Load texture
void texLoad(char *file) {
  // Load Image
  FILE *f;
  if ( (f=fopen(file ,"rb")) == NULL ) {
    printf("Error loading file!\n");
    exit(0);
  }
  fread(image, sizeof(image), 1, f);

  // Generate Mip Maps
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, SIZE, SIZE, GL_RGB, GL_UNSIGNED_BYTE, image );

  fclose(f);
}

// Set up lighting
void initLight() {
  glEnable (GL_LIGHTING);
//  glShadeModel (GL_FLAT);
  glShadeModel (GL_SMOOTH);
  
  // Point light
  glEnable (GL_LIGHT0);
  GLfloat light_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  
  // Spot light
  //glEnable(GL_LIGHT1);
  GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light1_position[] = { 5.0, 5.0, 5.0, 1.0 };
  GLfloat light1_direction[] = { -1.0, -1.0, -1.0 };

  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.);
  
  // Directional light
  glEnable(GL_LIGHT2);
  GLfloat light2_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat light2_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light2_specular[] = { 0.0, 1.0, 0.0, 1.0 };
  GLfloat light2_direction[] = { -1.0, -1.0, -1.0 };

  glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
  
  // Object material
  GLfloat DiffuseMaterial[] = {1.0, 1.0, 1.0}; 
  GLfloat SpecularMaterial[] = {1.0, 1.0, 1.0};
  GLfloat EmissiveMaterial[] = {0.0, 0.0, 0.0};
  GLfloat mShininess[] = {10.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SpecularMaterial);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseMaterial);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, EmissiveMaterial);
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
  glewInit();
  initGL();
  texLoad("image.rgb");

  // Lighting
  initLight();

  // Run the control loop
  glutDisplayFunc(display);
  glutIdleFunc(update);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return EXIT_SUCCESS;
}
