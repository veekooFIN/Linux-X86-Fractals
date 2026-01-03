# Linux-X86-Fractals
Hobby code development for Linux in C language - Fractals
---------------------------------------------------------------------------------------------------------------

In root there is old gfx opengl fractals and ascii fractals - currently after some update in linux don't work well

In folder fract_in_opengl there is updated gfx opengl fractals - updated but don't work well

In folder fract_in_sdl2 there is new gfx fractals working with SDL2

---------------------------------------------------------------------------------------------------------------

Gfx mandelbrot fractal set for Linux.

Gfx julia fractal set for Linux.

Gfx burnship fractal set for Linux.

Gfx versions use 256 iterations and greyscale palette.

Ascii mandelbrot fractal set for Linux.

Ascii julia fractal set for Linux.

---------------------------------------------------------------------------------------------------------------


---------------------------------------------------------------------------------------------------------------
C language code developed for Linux. 
---------------------------------------------------------------------------------------------------------------


---------------------------------------------------------------------------------------------------------------

Gfx mandelbrot uses floating point. Run from terminal and opens 1500x800 screen.

Gfx julia uses floating point. Run from terminal and opens 1500x800 screen.

Gfx burnship uses floating point. Run from terminal and opens 1500x800 screen.

Ascii mandelbrot uses floating point. Run in terminal screen 132x43.

Ascii julia uses floating point. Run in terminal screen 132x43.

---------------------------------------------------------------------------------------------------------------

Some tips

How to install OpenGL/Glut:

sudo apt-get install aptitude

sudo aptitude install freeglut3-dev

How to compile:

gcc gfxbrot.c -o gfxbrot -lGL -lGLU -lglut -lm

How to run:

From terminal ./gfxbrot
