/*                                                                                                                     
  __    _ ______  ______  __  __  _____  _____  ______  ____  ____   _   ______  _____  _____   ____  ____   _  ______  
 \  \  //|   ___||   ___||  |/ / /     \/     \|   ___||    ||    \ | | |   ___|/     \|     \ |    ||    \ | ||   ___| 
  \  \// |   ___||   ___||     \ |     ||     ||   ___||    ||     \| | |   |__ |     ||      \|    ||     \| ||   |  | 
   \__/  |______||______||__|\__\\_____/\_____/|___|   |____||__/\____| |______|\_____/|______/|____||__/\____||______| 
                                                                                                                        
                                                                                                                        */ 
/*----------------------------------------------------------------------+
 |                                                                      |
 |             gfxjuliazoom.c -- demonstrate fractal in gfx             |
 |                                                                      |
 +----------------------------------------------------------------------*/

#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define INITIAL_WIDTH  1200
#define INITIAL_HEIGHT 800
#define CX -0.8
#define CY 0.156

double view_left   = -1.70;
double view_right  =  1.70;
double view_bottom = -1.00;
double view_top    =  1.00;

int max_iter = 256;
double zoom_factor = 0.4;

void julia_pixel(double real0, double imag0, float *r, float *g, float *b)
{
    double x = real0; 
    double y = imag0;
    int iter = 0;

    while ((x*x + y*y <= 4.0) && (iter < max_iter))
    {
        double xtemp = x*x - y*y + CX;
        y = 2.0*x*y + CY;
        x = xtemp;
        iter++;
    }

    if (iter == max_iter)
    {
        *r = *g = *b = 0.0f;
    }
    else
    {
        double t = (double)iter / max_iter;
        *r = 0.8f + 0.2f * sin(13.0 * t + 0.8);
        *g = 0.4f + 0.3f * sin(17.0 * t + 2.1);
        *b = 0.1f + 0.7f * cos( 9.0 * t + 1.3);
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    int win_width = glutGet(GLUT_WINDOW_WIDTH);
    int win_height = glutGet(GLUT_WINDOW_HEIGHT);

    double dx = (view_right - view_left) / win_width;
    double dy = (view_top - view_bottom) / win_height;

    for (int x = 0; x < win_width; x++)
    {
        double real = view_left + x * dx;
        for (int y = 0; y < win_height; y++)
        {
            double imag = view_bottom + y * dy;

            float r, g, b;
            julia_pixel(real, imag, &r, &g, &b);

            glColor3f(r, g, b);
            glVertex2i(x, y);
        }
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

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        int win_width  = glutGet(GLUT_WINDOW_WIDTH);
        int win_height = glutGet(GLUT_WINDOW_HEIGHT);

        double norm_x = (double)x / win_width;
        double norm_y = (double)(win_height - y) / win_height; 
        
        double zoom_center_real = view_left   + norm_x * (view_right - view_left);
        double zoom_center_imag = view_bottom + norm_y * (view_top    - view_bottom);

        double width  = view_right - view_left;
        double height = view_top   - view_bottom;

        bool changed = false;

        if (button == GLUT_LEFT_BUTTON && (glutGetModifiers() & GLUT_ACTIVE_CTRL))
        {
            double new_width  = width  * zoom_factor;
            double new_height = height * zoom_factor;

            view_left   = zoom_center_real - new_width  * 0.5;
            view_right  = zoom_center_real + new_width  * 0.5;
            view_bottom = zoom_center_imag - new_height * 0.5;
            view_top    = zoom_center_imag + new_height * 0.5;
            changed = true;
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
            double new_width  = width  / zoom_factor;
            double new_height = height / zoom_factor;

            view_left   = zoom_center_real - new_width  * 0.5;
            view_right  = zoom_center_real + new_width  * 0.5;
            view_bottom = zoom_center_imag - new_height * 0.5;
            view_top    = zoom_center_imag + new_height * 0.5;
            changed = true;
        }

        if (changed) {
            glutPostRedisplay();
        }
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'r': case 'R':
            view_left   = -1.70; view_right  =  1.70;
            view_bottom = -1.00; view_top    =  1.00;
            max_iter = 256;
            glutPostRedisplay();
            break;
        case '+': case '=':
            max_iter += 64;
            printf("Max iterations: %d\n", max_iter);
            glutPostRedisplay();
            break;
        case '-': case '_':
            if (max_iter > 64) max_iter -= 64;
            printf("Max iterations: %d\n", max_iter);
            glutPostRedisplay();
            break;
        case 27: exit(0); break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(INITIAL_WIDTH, INITIAL_HEIGHT);
    glutCreateWindow("Julia - OpenGL");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    printf("Controls:\n");
    printf("  Ctrl + Left Mouse  → Zoom In\n");
    printf("  Right Mouse        → Zoom Out\n");
    printf("  + / -              → Increase/Decrease Iterations\n");
    printf("  R                  → Reset View\n");
    printf("  ESC                → Close\n\n");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}