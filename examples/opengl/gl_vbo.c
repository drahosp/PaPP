/*
 OpenGL visualization skeleton for displaying bitmap images. Just provide a GenerateImage function.
 Good starting point for all image processing exercises for parallel programming.

 Example use of VBO
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

#define DIST 1
float vertices[] = {
  // Quad 1
  -1,-1,DIST,
  1,-1,DIST,
  1,1,DIST,
  -1,1,DIST,
  // Quad 2
  -1,1,-DIST,
  1,1,-DIST,
  1,-1,-DIST,
  -1,-1,-DIST,
};

float texcoord[] = {
  0,1,
  1,1,
  1,0,
  0,0,
  0,0,
  0,1,
  1,1,
  1,0,
};

#define SIZE 512
pixel image[SIZE][SIZE];
GLuint texture, vbo_vertices, vbo_texcoord;

float t = 0;

// Initialize object buffers
void initVBO() {
  // Generate buffer for vertices
  glGenBuffers( 1, &vbo_vertices );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_vertices );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

  // Generate buffer for texture coordinates
  glGenBuffers( 1, &vbo_texcoord );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_texcoord );
  glBufferData( GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_STATIC_DRAW );
}

// Use VBO to display the object
void drawVBO() {
  // Turn on vertex/texcoord arrays
  glEnableClientState( GL_VERTEX_ARRAY );
  glEnableClientState( GL_TEXTURE_COORD_ARRAY );

  // Use vertex pointers from vbo_vertices
  glBindBuffer( GL_ARRAY_BUFFER, vbo_vertices );
  glVertexPointer( 3, GL_FLOAT, 0, 0);

  // Use texcoord pointers from vbo_texcoord
  glBindBuffer( GL_ARRAY_BUFFER, vbo_texcoord );
  glTexCoordPointer( 2, GL_FLOAT, 0, 0);

  // Draw
  glDrawArrays( GL_QUADS, 0, 8 );

  // Disable vertex/texcoord arrays
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
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

  // Draw object
//   glutSolidTeapot(1);
//  glutSolidCube(1);
//  glutSolidSphere(1,50,50);
    drawVBO();

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
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
  glEnable (GL_LIGHT0);
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
  initVBO();
  texLoad("image.rgb");

  // Lighting
  //initLight();

  // Run the control loop
  glutDisplayFunc(display);
  glutIdleFunc(update);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return EXIT_SUCCESS;
}
