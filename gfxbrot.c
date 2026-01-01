/*                                                                                                                     
  __    _ ______  ______  __  __  _____  _____  ______  ____  ____   _   ______  _____  _____   ____  ____   _  ______  
 \  \  //|   ___||   ___||  |/ / /     \/     \|   ___||    ||    \ | | |   ___|/     \|     \ |    ||    \ | ||   ___| 
  \  \// |   ___||   ___||     \ |     ||     ||   ___||    ||     \| | |   |__ |     ||      \|    ||     \| ||   |  | 
   \__/  |______||______||__|\__\\_____/\_____/|___|   |____||__/\____| |______|\_____/|______/|____||__/\____||______| 
                                                                                                                        
                                                                                                                        */ 
/*----------------------------------------------------------------------+
 |                                                                      |
 |             gfxbrot.c -- demonstrate fractal in gfx                  |
 |                                                                      |
 +----------------------------------------------------------------------*/
 
// Standard includes
#include <GL/glut.h>

#define WIDTH 1500
#define HEIGHT 800

//initialization
void init(void);
//does the drawing 
void display(void);

//initialization
void init( void )
{
  glClearColor( 0.0, 0.0, 0.0, 0.0 );	//get white background color
  glColor3f( 1.0f, 1.0f, 1.0f );	//set drawing color
  glPointSize( 1.0 );			//a dot is 1x1
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();			//replace current matrix with identity matrix
  gluOrtho2D( 0.0, WIDTH, 0.0, HEIGHT);
}

int mandelbrot(double real0, double imag0) {
  double realq, imagq; 
  double real, imag;
  int i;

  real = real0;
  imag = imag0;
  for (i = 0; i < 256; i++)
  {
    realq = (real * real);
    imagq = (imag * imag);

    if ((realq + imagq) > 4.0f) break;

    imag = (2.0f * real * imag) + imag0;
    real = realq - imagq + real0;
  }
  return i;
}

void display( void )
{
  int x, y, data;
  double col;
  double realmin, imagmin, realmax, imagmax;
  double deltareal, deltaimag, real0, imag0;  
    
  glClear( GL_COLOR_BUFFER_BIT );	//clear screen
  glBegin( GL_POINTS );			//draw points

  realmin = -2.0f;
  realmax = 0.8f;
  imagmin = -1.0f;
  imagmax = 1.0f; 
  
  deltareal = (double) (realmax - realmin) / (double) WIDTH;
  deltaimag = (double) (imagmax - imagmin) / (double) HEIGHT;

  real0 = realmin; 
  for(x = 0; x < WIDTH; x++ ) {
    imag0 = imagmax;
    for(y = 0; y < HEIGHT; y++ ) {
      data = mandelbrot(real0, imag0);
      col = data/256.0f;
      
      glColor3f(col,col,col);
      glVertex2i(x,y);
      
      imag0 -= deltaimag;
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
  glutInitWindowSize(WIDTH, HEIGHT);		//set window size on screen
  glutInitWindowPosition( 0, 0 ); 	//set window position on screen
  glutCreateWindow(argv[0]);		//open screen window
  init();
  glutDisplayFunc (display);		//points to display function
  glutMainLoop();			//go into perpetual loop
  return 0;
}

