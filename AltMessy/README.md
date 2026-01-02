# Linux-X86-Fractals
Hobby code development for Linux in C language - Fractals
---------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------

Here is OpenGL updated version if previous don't work

---------------------------------------------------------------------------------------------------------------

Some tips

How to install OpenGL/freeglut:

sudo apt update

sudo apt install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev

How to compile:

gcc gfxbrot.c -o gfxbrot -lglut -lGL -lGLU

How to run:

From terminal ./gfxbrot

---------------------------------------------------------------------------------------------------------------

Here is SDL2 version if OpenGL don't work

---------------------------------------------------------------------------------------------------------------

Some tips

How to install SDL2:

sudo apt update

sudo apt install libsdl2-dev

How to compile:

gcc gfxbrot_sdl.c -o gfxbrot_sdl -lSDL2

How to run:

From terminal ./gfxbrot_sdl
