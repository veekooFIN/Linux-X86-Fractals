# Linux-X86-Fractals
Hobby code development for Linux in C language - Fractals
---------------------------------------------------------------------------------------------------------------

Gfx mandelbrot fractal set for Linux.

Gfx julia fractal set for Linux.

Gfx versions use 256 iterations and greyscale palette.

Ascii mandelbrot fractal set for Linux.

Ascii julia fractal set for Linux.

---------------------------------------------------------------------------------------------------------------


---------------------------------------------------------------------------------------------------------------
C language code developed for Linux. 
Executable files added.
Runtime pictures added.

---------------------------------------------------------------------------------------------------------------


---------------------------------------------------------------------------------------------------------------

Gfx mandelbrot uses floating point. Run from terminal and opens 3840x2160 screen.

Gfx julia uses floating point. Run from terminal and opens 3840x2160 screen.

Ascii mandelbrot uses floating point. Run in terminal screen 200x50.

Ascii julia uses floating point. Run in terminal screen 200x50.

---------------------------------------------------------------------------------------------------------------

Some tips

How to install OpenGL/Glut:

sudo apt-get install aptitude

sudo aptitude install freeglut3-dev

How to compile:

gcc gfxbrot.c -o gfxbrot -lGL -lGLU -lglut -lm

How to run:

From terminal ./gfxbrot
