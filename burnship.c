/*----------------------------------------------------------------------+
 |                                                                      |
 |     burnship.c -- demonstrate fractal in gfx / quick and dirty       |
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

#define WIDTH 2000
#define HEIGHT 1000

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
  gluOrtho2D( 0.0, 2000.0, 0.0, 1000.0 );
}

int burnship(float real0, float imag0) {
  float realq, imagq; 
  float real, imag;
  int i;

  real = real0;
  imag = imag0;
  for (i = 0; i < 256; i++)
  {
    realq = (real * real);
    imagq = (imag * imag);

    if ((realq + imagq) > (float) 4) break;

    imag = abs(2.0 * real * imag) + imag0;
    real = realq - imagq + real0;
  }
  return i;
}

void display( void )
{
  int x, y, data;
  float col;
  float realmin, imagmin, realmax, imagmax;
  float deltareal, deltaimag, real0, imag0;  
    
  realmin = (float) (-2.5);
  realmax = (float) (2.5);
  imagmin = (float) (-2.5);
  imagmax = (float) (2.5); 
  
  deltareal = (float) (realmax - realmin) / (float) WIDTH;
  deltaimag = (float) (imagmax - imagmin) / (float) HEIGHT;

  glClear( GL_COLOR_BUFFER_BIT );	//clear screen
  glBegin( GL_POINTS );			//draw points

  real0 = realmin; 
  for(x = 0; x < WIDTH; x++ ) {
    imag0 = imagmin;
    for(y = 0; y < HEIGHT; y++ ) {
      data = burnship(real0, imag0);
      col = (256.0-data)/256.0;
      
      glColor3f(col,col,col);
      glVertex2i(x,y);
      
      imag0 += deltaimag;
    }
    real0 += deltareal;
  }
  glEnd();
  glFlush();				//send all output to screen
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);	//initialize toolkit
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );//set display mode: single bufferring, RGBA model
  glutInitWindowSize(2000, 1000);		//set window size on screen
  glutInitWindowPosition( 0, 0 ); 	//set window position on screen
  glutCreateWindow(argv[0]);		//open screen window
  init();
  glutDisplayFunc (display);		//points to display function
  glutMainLoop();			//go into perpetual loop
  return 0;
}

