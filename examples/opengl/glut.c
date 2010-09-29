#include <GL/glut.h>

// GLUT application skeleton
//function to initialize GLUT settings

void init()
{
    glClearColor(0,0,0,0);//(NEW) we define the background color here
}

//The function where all the drawing occurs
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);//Clear the screen


    glFlush();//Draw everything to the screen
    glutPostRedisplay();//Tell the program to refresh
}

void main(int argc, char ** argv)
{
    glutInit(&argc, argv);//initialize GLUT.
    glutInitWindowSize(800,600);//define the window size as 800 pixels wide and 600 pixels high    glutInitWindowPosition(10,50);//Set the window position at (10,50)
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);//Set the initial display mode
    glutCreateWindow("Lesson 1");//Create our window
    init();//call init()
    glutDisplayFunc(display);//tell GLUT what our display function is
    glutMainLoop();//Tell the program we are not done yet
}
