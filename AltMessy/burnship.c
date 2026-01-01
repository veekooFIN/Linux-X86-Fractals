/*                                                                                                                     
  __    _ ______  ______  __  __  _____  _____  ______  ____  ____   _   ______  _____  _____   ____  ____   _  ______  
 \  \  //|   ___||   ___||  |/ / /     \/     \|   ___||    ||    \ | | |   ___|/     \|     \ |    ||    \ | ||   ___| 
  \  \// |   ___||   ___||     \ |     ||     ||   ___||    ||     \| | |   |__ |     ||      \|    ||     \| ||   |  | 
   \__/  |______||______||__|\__\\_____/\_____/|___|   |____||__/\____| |______|\_____/|______/|____||__/\____||______| 
                                                                                                                        
                                                                                                                        */ 
/*----------------------------------------------------------------------+
 |                                                                      |
 |             burnship.c -- demonstrate fractal in gfx                 |
 |                                                                      |
 +----------------------------------------------------------------------*/
 
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define WIDTH 1500
#define HEIGHT 800

void init(void);
void display(void);
void reshape(int w, int h);

void init(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glPointSize(1.0f);
}

int burnship(double real0, double imag0) {
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

    imag = fabs(2.0f * real * imag) + imag0;
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
    
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POINTS);

  realmin = -1.8f;
  realmax = -1.7f;
  imagmin = -0.08f;
  imagmax = 0.01f; 
  
  deltareal = (double) (realmax - realmin) / (double) WIDTH;
  deltaimag = (double) (imagmax - imagmin) / (double) HEIGHT;

  real0 = realmin; 
  for(x = 0; x < WIDTH; x++ ) {
    imag0 = imagmax;
    for(y = 0; y < HEIGHT; y++ ) {
      data = burnship(real0, imag0);
      col = data/256.0f;
      
      glColor3f(col,col,col);
      glVertex2i(x,y);
      
      imag0 -= deltaimag;
    }
    real0 += deltareal;
  }
  glEnd();
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, w, 0.0, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Burning Ship Set - OpenGL/GLUT");

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0;
}