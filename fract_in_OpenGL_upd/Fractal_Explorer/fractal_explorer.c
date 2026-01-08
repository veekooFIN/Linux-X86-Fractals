/*                                                                                                                     
  __    _ ______  ______  __  __  _____  _____  ______  ____  ____   _   ______  _____  _____   ____  ____   _  ______  
 \  \  //|   ___||   ___||  |/ / /     \/     \|   ___||    ||    \ | | |   ___|/     \|     \ |    ||    \ | ||   ___| 
  \  \// |   ___||   ___||     \ |     ||     ||   ___||    ||     \| | |   |__ |     ||      \|    ||     \| ||   |  | 
   \__/  |______||______||__|\__\\_____/\_____/|___|   |____||__/\____| |______|\_____/|______/|____||__/\____||______| 
                                                                                                                         
                                                                                                                         */ 
/*----------------------------------------------------------------------+
 |                                                                      |
 |             fractal_explorer.c -- unified fractal explorer            |
 |                                                                      |
 +----------------------------------------------------------------------*/

#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define INITIAL_WIDTH  1200
#define INITIAL_HEIGHT 800

typedef enum {
    FRACTAL_MANDELBROT,
    FRACTAL_JULIA,
    FRACTAL_BURNING_SHIP
} FractalType;

typedef struct {
    FractalType type;
    char name[32];
    double view_left, view_right, view_bottom, view_top;
    double julia_cx, julia_cy;
} FractalConfig;

FractalConfig fractals[] = {
    {FRACTAL_MANDELBROT, "Mandelbrot", -2.50, 1.00, -1.40, 1.40, 0.0, 0.0},
    {FRACTAL_JULIA, "Julia", -1.70, 1.70, -1.00, 1.00, -0.8, 0.156},
    {FRACTAL_BURNING_SHIP, "Burning Ship", -1.8, -1.7, -0.08, 0.01, 0.0, 0.0}
};

int current_fractal = 0;
int max_iter = 256;
double zoom_factor = 0.4;

void mandelbrot_pixel(double real0, double imag0, float *r, float *g, float *b)
{
    double x = 0.0, y = 0.0;
    int iter = 0;

    while ((x*x + y*y <= 4.0) && (iter < max_iter))
    {
        double xtemp = x*x - y*y + real0;
        y = 2.0*x*y + imag0;
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

void julia_pixel(double real0, double imag0, float *r, float *g, float *b)
{
    double x = real0; 
    double y = imag0;
    int iter = 0;
    FractalConfig *fc = &fractals[current_fractal];

    while ((x*x + y*y <= 4.0) && (iter < max_iter))
    {
        double xtemp = x*x - y*y + fc->julia_cx;
        y = 2.0*x*y + fc->julia_cy;
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

void burnship_pixel(double real0, double imag0, float *r, float *g, float *b)
{
    double x = 0.0, y = 0.0;
    int iter = 0;

    while ((x*x + y*y <= 4.0) && (iter < max_iter))
    {
        double xtemp = x*x - y*y + real0;
        y = fabs(2.0*x*y) + imag0; 
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
    FractalConfig *fc = &fractals[current_fractal];

    double dx = (fc->view_right - fc->view_left) / win_width;
    double dy = (fc->view_top - fc->view_bottom) / win_height;

    for (int x = 0; x < win_width; x++)
    {
        double real = fc->view_left + x * dx;
        for (int y = 0; y < win_height; y++)
        {
            double imag;
            if (fc->type == FRACTAL_BURNING_SHIP)
                imag = fc->view_top - y * dy;
            else
                imag = fc->view_bottom + y * dy;

            float r, g, b;
            
            switch(fc->type)
            {
                case FRACTAL_MANDELBROT:
                    mandelbrot_pixel(real, imag, &r, &g, &b);
                    break;
                case FRACTAL_JULIA:
                    julia_pixel(real, imag, &r, &g, &b);
                    break;
                case FRACTAL_BURNING_SHIP:
                    burnship_pixel(real, imag, &r, &g, &b);
                    break;
            }

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
        FractalConfig *fc = &fractals[current_fractal];

        double norm_x = (double)x / win_width;
        double norm_y = (double)(win_height - y) / win_height; 
        
        double zoom_center_real = fc->view_left   + norm_x * (fc->view_right - fc->view_left);
        double zoom_center_imag = fc->view_bottom + norm_y * (fc->view_top    - fc->view_bottom);

        double width  = fc->view_right - fc->view_left;
        double height = fc->view_top   - fc->view_bottom;

        bool changed = false;

        if (button == GLUT_LEFT_BUTTON && (glutGetModifiers() & GLUT_ACTIVE_CTRL))
        {
            double new_width  = width  * zoom_factor;
            double new_height = height * zoom_factor;

            fc->view_left   = zoom_center_real - new_width  * 0.5;
            fc->view_right  = zoom_center_real + new_width  * 0.5;
            fc->view_bottom = zoom_center_imag - new_height * 0.5;
            fc->view_top    = zoom_center_imag + new_height * 0.5;
            changed = true;
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
            double new_width  = width  / zoom_factor;
            double new_height = height / zoom_factor;

            fc->view_left   = zoom_center_real - new_width  * 0.5;
            fc->view_right  = zoom_center_real + new_width  * 0.5;
            fc->view_bottom = zoom_center_imag - new_height * 0.5;
            fc->view_top    = zoom_center_imag + new_height * 0.5;
            changed = true;
        }

        if (changed) {
            glutPostRedisplay();
        }
    }
}

void reset_fractal(int fractal_index)
{
    FractalConfig *fc = &fractals[fractal_index];
    
    switch(fc->type)
    {
        case FRACTAL_MANDELBROT:
            fc->view_left = -2.50; fc->view_right = 1.00;
            fc->view_bottom = -1.40; fc->view_top = 1.40;
            break;
        case FRACTAL_JULIA:
            fc->view_left = -1.70; fc->view_right = 1.70;
            fc->view_bottom = -1.00; fc->view_top = 1.00;
            break;
        case FRACTAL_BURNING_SHIP:
            fc->view_left = -1.8; fc->view_right = -1.7;
            fc->view_bottom = -0.08; fc->view_top = 0.01;
            break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '1':
            current_fractal = 0;
            reset_fractal(current_fractal);
            glutSetWindowTitle("Fractal Explorer - Mandelbrot");
            printf("Switched to Mandelbrot\n");
            glutPostRedisplay();
            break;
        case '2':
            current_fractal = 1;
            reset_fractal(current_fractal);
            glutSetWindowTitle("Fractal Explorer - Julia");
            printf("Switched to Julia\n");
            glutPostRedisplay();
            break;
        case '3':
            current_fractal = 2;
            reset_fractal(current_fractal);
            glutSetWindowTitle("Fractal Explorer - Burning Ship");
            printf("Switched to Burning Ship\n");
            glutPostRedisplay();
            break;
        case 'r': case 'R':
            reset_fractal(current_fractal);
            printf("Reset %s view\n", fractals[current_fractal].name);
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
        case 'h': case 'H':
            printf("\n=== Fractal Explorer Controls ===\n");
            printf("  1                  → Mandelbrot\n");
            printf("  2                  → Julia\n");
            printf("  3                  → Burning Ship\n");
            printf("  Ctrl + Left Mouse  → Zoom In\n");
            printf("  Right Mouse        → Zoom Out\n");
            printf("  + / -              → Increase/Decrease Iterations\n");
            printf("  R                  → Reset Current View\n");
            printf("  H                  → Show Help\n");
            printf("  ESC                → Close\n\n");
            break;
        case 27: exit(0); break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(INITIAL_WIDTH, INITIAL_HEIGHT);
    glutCreateWindow("Fractal Explorer - Mandelbrot");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    printf("=== Fractal Explorer ===\n");
    printf("Currently showing: %s\n\n", fractals[current_fractal].name);
    printf("Controls:\n");
    printf("  1                  → Mandelbrot\n");
    printf("  2                  → Julia\n");
    printf("  3                  → Burning Ship\n");
    printf("  Ctrl + Left Mouse  → Zoom In\n");
    printf("  Right Mouse        → Zoom Out\n");
    printf("  + / -              → Increase/Decrease Iterations\n");
    printf("  R                  → Reset Current View\n");
    printf("  H                  → Show Help\n");
    printf("  ESC                → Close\n\n");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}