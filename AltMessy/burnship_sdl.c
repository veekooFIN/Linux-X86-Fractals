//sudo apt update
//sudo apt install libsdl2-dev

//gcc gfxbrot_sdl.c -o gfxbrot_sdl -lSDL2

/*                                                                                                                     
  __    _ ______  ______  __  __  _____  _____  ______  ____  ____   _   ______  _____  _____   ____  ____   _  ______  
 \  \  //|   ___||   ___||  |/ / /     \/     \|   ___||    ||    \ | | |   ___|/     \|     \ |    ||    \ | ||   ___| 
  \  \// |   ___||   ___||     \ |     ||     ||   ___||    ||     \| | |   |__ |     ||      \|    ||     \| ||   |  | 
   \__/  |______||______||__|\__\\_____/\_____/|___|   |____||__/\____| |______|\_____/|______/|____||__/\____||______| 
                                                                                                                        
                                                                                                                        */ 

/*----------------------------------------------------------------------+
 |                                                                      |
 |             gfxbrot_sdl.c -- demonstrate fractal in gfx              |
 |                                                                      |
 +----------------------------------------------------------------------*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 1500
#define HEIGHT 800

int burnship(double real0, double imag0) {
    double real = real0;
    double imag = imag0;
    double realq, imagq;
    int i;

    for (i = 0; i < 256; i++) {
        realq = real * real;
        imagq = imag * imag;
        if ((realq + imagq) > 4.0) break;
        imag = fabs(2.0 * real * imag) + imag0;
        real = realq - imagq + real0;
    }
    return i;
}

void render(SDL_Renderer *renderer) {
    double realmin = -1.8, realmax = -1.7;
    double imagmin = -0.08, imagmax = 0.01;
    double deltareal = (realmax - realmin) / WIDTH;
    double deltaimag = (imagmax - imagmin) / HEIGHT;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            double r0 = realmin + x * deltareal;
            double i0 = imagmin + y * deltaimag;
            int data = burnship(r0, i0);

            if (data < 256) {
                int col = data; // Harmaasävy 0-255
                SDL_SetRenderDrawColor(renderer, col, col, col, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    SDL_Window *window = SDL_CreateWindow("Burning Ship SDL2",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) return 1;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    render(renderer);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = 1;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}