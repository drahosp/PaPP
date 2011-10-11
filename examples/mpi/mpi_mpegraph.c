/*
A simple MPI example program that uses a number of procedures in
the MPE graphics library. See the comments in the code for a
description of what the program does.
*/

#include <mpi.h>
#define MPE_GRAPHICS   /* You need this to use the MPE graphics routines */
#include "mpe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Use an external random number generator */
extern int rand();

static MPE_XGraph graph;         /* Handle to the graphics window */
static char *displayname = "";  /* String for DISPLAY environment variable */

/* Width and height of window in pixels */
static int width = 950, height = 400;
int err;

main(int argc, char* argv[]) {
  int me, np, i;
  int x, y, w, h;
  int x1, y1, x2, y2;
  int color, button, pressed;
  int space, num_colors;
  MPE_Color col_array[64];      /* Color array */
  MPE_Point points[80];         /* Array to store 80 pixels in */
  
  w=10; h=20;                   /* Width and height of colored rectangles */

  MPI_Init (&argc, &argv);              /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &np);   /* Get nr of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);   /* Get own id */
  
  /* Check that we have exactly two processes */
  if (np != 2) {
    if (me == 0) printf("You have to run this program with 2 processes\n");
    MPI_Finalize();
    exit(0);
  }
  
  if (me == 0) printf("Opening a graphics window\n") ;
  
  /* Open the graphics window in position (0,0) on the display */
  MPE_Open_graphics(&graph, MPI_COMM_WORLD, displayname, 0, 0, width, height, 0 );

  /* Get number of colors */
  MPE_Num_colors(graph, &num_colors);

  if (me == 0) {
    printf("Number of colors = %d,\n", num_colors) ;
  }
  
  /* Processor 0 puts a pink rectangle of size 100*240 in position (0,0) */
  if (me==0) {
    MPE_Fill_rectangle( graph, 0, 0, 100, 240, MPE_PINK );
    MPE_Update( graph );
  }
  
  /* Both processors draws one rectangle for each colour, placed in a row */
  x=0; y=(me)*50; space=2;
  for (color=0; color<num_colors; color++) {
    x=x+w+space;         /* Set position of rectangles */
    MPE_Fill_rectangle( graph, x, y, w, h, color );
  }
  MPE_Update( graph );
  
  /* Create a new color array with 64 colours */
  MPE_Make_color_array(graph, 64, col_array);
  /* Get number of colors */
  MPE_Num_colors(graph, &num_colors);
  if ( me == 0 ) {
    printf("Number of colors = %d,\n", num_colors) ;
  }
  
  /* Both processors draw one rectangle for each colour, placed in a row */
  x=0; y=(me+1)*100;
  for (color=0; color<num_colors; color++) {
    x=x+w+space;         /* Set position of rectangles */
    MPE_Fill_rectangle( graph, x, y, w, h, color );
  }
  MPE_Update( graph );
  
  /* Set lines to be 2 pixels thick */
  MPE_Line_thickness(graph, 2);
  /* Both processes draw a line, each with different color */
  MPE_Draw_line(graph, 300+(20*me), 10+(20*me), 800+(20*me), 70+(20*me), 30+(20*me));
  MPE_Update( graph );
  
  /* Both processes draw a circle of different size and color */
  MPE_Draw_circle(graph, 100+(50*me), 300+(50*me), 20+(5*me), num_colors-(20*me));
  MPE_Update(graph);
  
  /* Both processes draw 50 pixels with different colors */
  for (i=0; i<50; i++) {
    int rx, ry;
    rx = 200+rand()%15;       /* Random positions where to draw points */
    ry = 300+rand()%15+40*me;
    MPE_Draw_point(graph, rx, ry, num_colors-(20*me));
  }
  MPE_Update(graph);
  
  /* Build 80 pixels with randomly chosen colors in the array points */
  for (i=0; i<80; i++) {
    points[i].x = 250+rand()%15;        /* Set x coordinate */
    points[i].y = 300+rand()%15+40*me;  /* Set y coordinate */
    points[i].c = rand()%num_colors;    /* Set a color */
  }
  MPE_Draw_points(graph, points, 80);   /* Draw all 80 points */
  MPE_Update(graph);
  
  /* Draw a text string */
  MPE_Draw_string(graph, 400, 250, MPE_BLACK, "Hello from MPE_Draw_string");
  MPE_Update(graph);
  
  /* Wait until the user selects a region of the graph, using button 1 */
  /* Print out the coordinates of the selected region and draw a black */
  /* rectangle around the selected region */
  
  button=1;   /* Use button 1 */
  if (me==0) {
    printf("Select a region of the graph using mouse button %d\n", button);
    MPE_Get_drag_region( graph, button, MPE_DRAG_RECT, &x1, &y1, &x2, &y2 );
    
    printf("Selected region is (%d,%d) to (%d,%d)\n", x1, y1, x2, y2);
    
    /* Draw a black rectangle around the selected area */
    /* This should of course be implemented in a procedure MPE_Draw_rectangle */
    /* Maybe I will write one some day ... */
    MPE_Draw_line(graph, x1, y1, x1, y2, MPE_BLACK);
    MPE_Draw_line(graph, x1, y2, x2, y2, MPE_BLACK);
    MPE_Draw_line(graph, x2, y2, x2, y1, MPE_BLACK);
    MPE_Draw_line(graph, x2, y1, x1, y1, MPE_BLACK);
    MPE_Update(graph);
  }
  
  /* Wait until the user presses a mouse button */
  if (me == 0) {
    printf("Press a mouse button to terminate\n");
    MPE_Get_mouse_press(graph, &x, &y, &button);
    printf("User clicked button %d in position (%d,%d)\n", button, x, y);
  }
  
  /* We can also poll the mouse buttons with MPE_Iget_mouse_pressed */
  
  /*   pressed = 0;                                                           */
  /*   if (me == 0) {                                                          */
  /*     do {                                                                  */
  /*       MPE_Iget_mouse_press(graph, &x, &y, &button, &pressed);             */
  /*     } while (!pressed);                                                   */
  /*     printf("User clicked button %d in position (%d,%d)\n", button, x, y); */
  /*   }                                                                       */
  
  
  /* Wait here for terminationn signal from process 0 */
  /* MPI_Barrier(MPI_COMM_WORLD); */
  
  /* Close the graphics window and terminate */
  MPE_Close_graphics(&graph);
  MPI_Finalize();
  exit(0);
}
