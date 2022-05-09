/*----------------------------------------------------------------------+
 |                                                                      |
 |     gfxbrot.c -- demonstrate fractal in gfx / quick and dirty        |
 |                                                                      |
 +----------------------------------------------------------------------*/
 
// Standard includes
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

#define HEIGHT 2160
#define WIDTH 3840
#define SCALE 1.6
#define YSTEP 1
#define XSTEP 1

//initialization
void init(void);
//does the drawing 
void display(void);

//initialization
void init( void )
{
  glClearColor( 1.0, 1.0, 1.0, 0.0 );	//get white background color
  glColor3f( 0.0f, 0.0f, 0.0f );	//set drawing color
  glPointSize( 1.0 );			//a dot is 1x1
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();			//replace current matrix with identity matrix
  gluOrtho2D( 0.0, 3840.0, 0.0, 2160.0 );
}

int mandelbrot(float x, float y) {
 float zz;
 float a;
 float b;
 float a2;
 float b2;
 float atemp;
 int i;
  
  a = 0;
  b = 0;  
  i = 0;
  while (i < 256)
  {
    a2 = a * a;
    b2 = b * b;
    zz = a2 + b2;
    if(zz > 4) break;
    
    atemp = a2 - b2 + x;
    b = 2.0 * a * b + y;
    a = atemp;
    i++;
  }
  return i;
}

void display( void )
{
  int x, y, data;
  float sx, sy, col;
  
  glClear( GL_COLOR_BUFFER_BIT );	//clear screen
  glBegin( GL_POINTS );			//draw points
    for(y = 0; y < HEIGHT; y = y + YSTEP ) {
      for(x = 0; x < WIDTH; x = x + XSTEP ) {
        sx = -0.7 + (SCALE * (WIDTH/2.0 - x) / (WIDTH/2.0))*(-1);
        sy = (SCALE * (HEIGHT/2.0 - y) / (HEIGHT/2.0))*(-0.75);
        data = mandelbrot(sx, sy);
        col = (256.0-data)/256.0;
      
        glColor3f(col,col,col);
        glVertex2i(x,y);
      }
    }  
  glEnd();
  glFlush();				//send all output to screen
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);	//initialize toolkit
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );//set display mode: single bufferring, RGBA model
  glutInitWindowSize(3840, 2160);		//set window size on screen
  glutInitWindowPosition( 0, 0 ); 	//set window position on screen
  glutCreateWindow(argv[0]);		//open screen window
  init();
  glutDisplayFunc (display);		//points to display function
  glutMainLoop();			//go into perpetual loop
  return 0;
}

