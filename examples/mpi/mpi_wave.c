/* 
   This program simulates a vibrating string suspended at its leftmost
   and rightmost point. The program is based on the problem described
   in Chapter 5 of Fox et al., Solving Problems on Concurrent Processors,
   vol 1, 1988.

   The string is decomposed into N points. Each processor is
   responsible for updating the amplitude of N/p points over time,
   where N is the number of points and p is the number of processors
   At each iteration, the processor exchanges boundary points with
   its two nearest neighbors.

   The program is based on an example program from Cornell, but
   heavily modified by Mats Aspnäs, for instance graphical output was
   added.

   Compile with 'mpicc -mpe=graphics MPI_wave.c -o MPI_wave -lm'
*/

#include <mpi.h>
#define MPE_GRAPHICS   /* You need this to use the MPE graphics routines */
#include <mpe.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXPROC 8
#define MAXPOINTS 1600

/* Prototypes */
int min(int a, int b);
void read_input(int id);
void init_line(void);
void update_graphics(int *pressed);
void update(int left, int right);

const double PI = 3.141592653589793238462643;
const int datatag = 42;
int id,                    /* Process ID */
    nproc,                 /* Number of processes */
    steps,                 /* Number of time steps */
    pressed,               /* Termination signal */
    N,                     /* Total nr. of points along the string */
    M,                     /* Number of points handled by this processor */
    first;                 /* Index of first point handled by this processor */

int left, right;           /* Left and right neighbour */
int height, width;         /* Height and width of the graphics window */

double *X,                 /* Values at time t */
       *X_old,             /* Values at time (t-deltat) */
       *X_new;             /* Values at time (t+deltat) */

MPI_Status status;

MPE_Point *points1, *points2;    /* Pixels to update */
static MPE_XGraph graph;         /* Handle to the graphics window */
static char *displayname = 0;    /* Null string means use value of DISPLAY */


int min(int a, int b) {
  return ((a<b) ? a : b);
}

/* Read the total number of points N and broadcast this to all processes */
void read_input(int id) {
  if (id == 0) {
    N = 0;
    while (N<nproc || N>MAXPOINTS) {
      printf("\nGive the number of points (between %d and %d): ", 
	     nproc, MAXPOINTS);
      scanf("%d", &N);
    }
  }
  /* Broadcast total number of points */
  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
}


/* Allocate space for the arrays, calculate the number of points for */
/* each process and initialize the array X to a sinus curve          */
void init_line(void) {
  int nmin, nleft, i, start;
  double fac;

  nmin = N/nproc;    /* Nr of points per process */
  nleft = N%nproc;   /* Nr of points left over */
  fac = 2.0 * PI;
  //fac = PI;

  /* Count how many points each process gets */
  if (id < nleft) {       /* Assign the left over points to the */
    M = nmin+1;           /* nleft first processes */
  } else {
    M = nmin;
  }
  
  /* Count which point in the global array is the first of this process */
  first = id*nmin + min(id,nleft);
  //printf ("Process %d, first = %d, last = %d, length = %d\n", id, first, first+M-1, M);

  M += 2;   /* We need two ghost points for values from left and right */
  /* Allocate memory for the arrays X, X_old and X_new */
  X = (double *) malloc(M*sizeof(double));
  X_old = (double *) malloc(M*sizeof(double));
  X_new = (double *) malloc(M*sizeof(double));

  /* Allocate memory for the pixels to be displayed.
     We will only use the pixels from 1 to M-2 when we draw them.
  */
  points1 = (MPE_Point *) malloc((M)*sizeof(MPE_Point));
  points2 = (MPE_Point *) malloc((M)*sizeof(MPE_Point));

  /* Initialize the local array X to a sinus curve */
  start = first;
  for (i=1; i<=M-2; i++) {
    X[i] = sin(fac* (double)start++/(double)N);
    X_old[i] = X[i];
  }
  X[0] = X[1]; X[M-1] = X[M-2];
  X_old[0] = X_old[1]; X_old[M-1] = X_old[M-2];
}

/* Open a shared graphics window of size wihth*height pixels */
void init_graphics(void) {
  width = N;           /* Width of graphics window */
  height = N/2+20;     /* Height of graphics window */
  pressed = 0;
  
  /* All processes open a shared graphics window */
  MPE_Open_graphics(&graph, MPI_COMM_WORLD, displayname, 0, 0, width, height, 0 );
  
  /* Make the window black */
  if (id == 0) { 
    MPE_Fill_rectangle(graph, 0,0, width,height, MPE_BLACK);
    MPE_Update(graph);
    printf ("\nClick in the graphics window to quit\n\n");
  }
}


void update_graphics(int *pressed) {
  int xpos, ypos, color;          /* Coordinates and color for a point to draw */
  int i, button, start;
  const double scaley = (double)height/2-20;  /* Scaling factor for y-coord */
  color = id+2;   /* Each process uses a different color */
  start = first;
  for (i=1; i<=M-2; i++) {
    /* First the old points that should be removed from the screen */
    xpos = start;  /* Start from the first point of this process */
    ypos = height/2 + (int)(scaley*X_old[i]);
    points1[i].x = xpos;
    points1[i].y = ypos;
    points1[i].c = MPE_BLACK;
    /* Then the new points that should be drawn on the screen */
    ypos = height/2 + (int)(scaley*X[i]);
    points2[i].x = xpos;
    points2[i].y = ypos;
    points2[i].c = color;
    start++;
  }

  /* Remove the old points */
  /* The arrays points1 and points2 start from element 1 */
  MPE_Draw_points (graph, &points1[1], M-2);
  /* Draw the new points */
  MPE_Draw_points (graph, &points2[1], M-2);
  MPE_Update(graph);

  /* Quit if the user clicks in the window */
  if (id == 0) {
    MPE_Iget_mouse_press(graph, &xpos, &ypos, &button, pressed);
    /* If you want to step through the execution 10 step at a time.
       then use the following code instead. Button3 terminates. */
    /*
    if (steps%10 == 0) {
      MPE_Get_mouse_press(graph, &xpos, &ypos, &button);
      if (button == MPE_BUTTON3) *pressed=1;
    }
    */
  }
  /* Broadcast the termination signal to all processes */
  MPI_Bcast(pressed, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

/* The processes update their points in X and display them */
/* in the graphics window                                  */

void update(int left, int right) {
/* Constants used in the formula for updating a point */
  const double deltat = 0.3;      /* The length of the time step */
  const double c = 1.0;           /* The constant used to calculate tau */
  const double deltax = 1.0;      /* The spacing between points */
  double tau, sqtau;              /* Tau and tau to the square  */
  int i;
  
  /* Calculate tau and tau to the power of 2 */
  tau = (c * deltat / deltax);
  sqtau = tau * tau;
  steps = 0;         /* Step counter */
  
  /* Loop until the user clicks in the window */
  while (!pressed) {
    
    steps++;         /* Count number of steps */
    /* Send to the left and receive from the right */
    MPI_Sendrecv(&X[1], 1, MPI_DOUBLE, left, datatag, &X[M-1],
		 1, MPI_DOUBLE, right, datatag, MPI_COMM_WORLD, &status);
    /* MPI_Send(&X[1], 1, MPI_DOUBLE, left, datatag, MPI_COMM_WORLD); */
    /* MPI_Recv(&X[M-1], 1, MPI_DOUBLE, right, datatag, MPI_COMM_WORLD, &status); */
    
    /* Send to the right and receive from the left */
    MPI_Sendrecv(&X[M-2], 1, MPI_DOUBLE, right, datatag, &X[0],
		 1, MPI_DOUBLE, left, datatag, MPI_COMM_WORLD, &status);
    /* MPI_Send(&X[M-2], 1, MPI_DOUBLE, right, datatag, MPI_COMM_WORLD); */
    /* MPI_Recv(&X[0], 1, MPI_DOUBLE, left, datatag, MPI_COMM_WORLD, &status); */

    
    /* Calculate new values using the wave equation */
    for (i=1; i<=M-2; i++) {
      X_new[i] = (2.0*X[i]) - X_old[i] + (sqtau*(X[i-1] - (2.0*X[i]) + X[i+1]));
    }

    /* Copy new values into X and previous values into X_old */
    for (i = 1; i<=M-2; i++) {
      X_old[i] = X[i];
      X[i] = X_new[i];
    }
    update_graphics(&pressed);
  }
}


int main(int argc, char **argv) {

  double start_time, stop_time;
  
  /* Initialize MPI */
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);     /* Get own id */
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);  /* Get number of processes */

  if (id == 0) {
    printf ("Wave program running on %d processors\n", nproc);
  }

  /* Determine the left and right neighbors */
  left  = (id == 0) ? MPI_PROC_NULL : id-1;
  right = (id == nproc-1) ? MPI_PROC_NULL : id+1;
  /* printf("Process %d, left = %d, right = %d\n", id, left, right); */

  /* Read the number of points N and send this to all processes */
  read_input(id);
  /* Initialize the line to a sinus wave */
  init_line();
  /* Open a graphics window for output */
  init_graphics();
  
  /* Update the values along the line */
  start_time = MPI_Wtime();
  update(left, right);
  stop_time =  MPI_Wtime();

  if (id == 0) printf ("%d steps simulated in %3.1f seconds\n\n", \
				steps, stop_time-start_time);

  /* Close the graphics window */
  MPE_Close_graphics (&graph);

  /* Free storage */
  free(X);   free(X_old);  free(X_new);
  free(points1);  free(points2);

  /* Exit */
  MPI_Finalize();
  exit(0);
}
