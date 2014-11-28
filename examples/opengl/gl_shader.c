/*
OpenGL simple shader demonstration.
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#ifdef __APPLE__
// On apple we do not need glew
#define glewInit()
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#else
// On other platforms use glew for compatibility
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif

#define SIZE 512

// Shader sources
char* vertexSource;
char* fragmentSource;

GLfloat vertices[] = {
  -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
};

GLuint elements[] = {
  0, 1, 2,
  2, 3, 0
};

float t = 0;
GLuint vbo, vao, ebo, vertexShader, fragmentShader, shaderProgram, status;

// Load a text file as string
char* loadFile(char* filename) {
  char * buffer = 0;
  long length;
  FILE * f = fopen (filename, "rb");

  if (f)
  {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = malloc (length+1);
    if (buffer)
    {
      fread (buffer, 1, length, f);
    }
    buffer[length]=0;
    fclose (f);
  }
  return buffer;
}

void init() {
  vertexSource = loadFile("simple_vs.glsl");
  fragmentSource = loadFile("simple_fs.glsl");

  // Vertex Array Object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create an element array
  glGenBuffers(1, &ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  // Create and compile the vertex shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if (status!=GL_TRUE) {
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    printf("Vertex Shader Error:\n%s\n",buffer);
    exit(0);
  }

  // Create and compile the fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (status!=GL_TRUE) {
    char buffer[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
    printf("Fragment Shader Error:\n%s\n",buffer);
    exit(0);
  }

  // Link the vertex and fragment shader into a shader program
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glBindFragDataLocation(shaderProgram, 0, "outColor");

  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

void display() {
  // Clear the screen to black
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw a rectangle from the 2 triangles using 6 indices
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
  glutInitDisplayMode(GLUT_3_2_CORE_PROFILE|GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
  glutCreateWindow("OpenGL Window");

  printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
  // Set up OpenGL state
  glewInit();
  init();

  // Run the control loop
  glutDisplayFunc(display);
  glutIdleFunc(update);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return EXIT_SUCCESS;
}
