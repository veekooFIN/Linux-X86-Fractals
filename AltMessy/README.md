# Linux-X86-Fractals
Hobby code development for Linux in C language - Fractals
---------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------

When there is a problem with OpenGL/Glut graphics. Made many tries to fix it, but no luck yet. Here is different versions.

---------------------------------------------------------------------------------------------------------------

Some tips

How to install OpenGL/freeglut:

sudo apt update

sudo apt install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev

How to compile:

gcc gfxbrot.c -o gfxbrot -lglut -lGL -lGLU

How to run:

From terminal ./gfxbrot
