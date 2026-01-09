/*                                                                                                                      
  __    _ ______  ______  __  __  _____  _____  ______  ____  ____   _   ______  _____  _____   ____  ____   _  ______  
 \  \  //|   ___||   ___||  |/ / /     \/     \|   ___||    ||    \ | | |   ___|/     \|     \ |    ||    \ | ||   ___| 
  \  \// |   ___||   ___||     \ |     ||     ||   ___||    ||     \| | |   |__ |     ||      \|    ||     \| ||   |  | 
   \__/  |______||______||__|\__\\_____/\_____/|___|   |____||__/\____| |______|\_____/|______/|____||__/\____||______| 
                                                                                                                         
                                                                                                                         */ 
/*----------------------------------------------------------------------+
 |                                                                      |
 |  fractal_explorer_color.c -- Beautiful colored fractal explorer        |
 |    Interactive zoom with mathematical color palette mapping         |
 +----------------------------------------------------------------------*/

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define WIDTH 1500
#define HEIGHT 800
#define MAX_ITERATIONS 256
#define NUM_THREADS 4
#define MAX_ZOOM_HISTORY 50
#define MIN_ZOOM 1e-10
#define MAX_ZOOM 1e10
#define NUM_PALETTES 6

typedef enum {
    FRACTAL_MANDELBROT,
    FRACTAL_JULIA,
    FRACTAL_BURNING_SHIP
} FractalType;

typedef enum {
    PALETTE_RAINBOW,
    PALETTE_FIRE,
    PALETTE_OCEAN,
    PALETTE_ELECTRIC,
    PALETTE_PASTEL,
    PALETTE_MONOCHROME
} ColorPalette;

typedef struct {
    double realmin, realmax, imagmin, imagmax;
    char name[32];
} FractalParams;

typedef struct {
    double realmin, realmax, imagmin, imagmax;
    FractalType type;
} ZoomState;

typedef struct {
    int start_y, end_y;
    FractalType type;
    ZoomState zoom;
    double deltareal, deltaimag;
    Uint32 *pixels;
    int completed;
    ColorPalette palette;
} ThreadData;

// Mouse interaction state
typedef struct {
    int dragging;
    int start_x, start_y;
    int curr_x, curr_y;
    int show_zoom_box;
} MouseState;

FractalParams fractal_params[] = {
    {-2.0, 0.8, -1.0, 1.0, "Mandelbrot Set"},
    {-1.7, 1.7, -1.0, 1.0, "Julia Set"},
    {-1.8, -1.7, -0.08, 0.01, "Burning Ship"}
};

#define JULIA_CX -0.8
#define JULIA_CY 0.156
#define ZOOM_FACTOR 0.5

ZoomState zoom_history[MAX_ZOOM_HISTORY];
int history_index = 0;
int history_count = 0;
ColorPalette current_palette = PALETTE_RAINBOW;

// Mathematical color mapping functions
inline Uint32 hsv_to_rgb(double h, double s, double v) {
    double c = v * s;
    double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m = v - c;
    
    double r = 0, g = 0, b = 0;
    int hi = (int)(h / 60.0) % 6;
    
    switch (hi) {
        case 0: r = c; g = x; b = 0; break;
        case 1: r = x; g = c; b = 0; break;
        case 2: r = 0; g = c; b = x; break;
        case 3: r = 0; g = x; b = c; break;
        case 4: r = x; g = 0; b = c; break;
        case 5: r = c; g = 0; b = x; break;
    }
    
    Uint8 ur = (Uint8)((r + m) * 255);
    Uint8 ug = (Uint8)((g + m) * 255);
    Uint8 ub = (Uint8)((b + m) * 255);
    
    return (0xFF << 24) | (ur << 16) | (ug << 8) | ub;
}

inline Uint32 get_color_rainbow(int iterations, int max_iterations) {
    if (iterations == max_iterations) return 0xFF000000;
    
    double smooth = iterations + 1.0 - log(log(2)) / log(2);
    double hue = fmod(smooth * 4.0, 360.0);
    double sat = 0.8;
    double val = iterations < max_iterations ? 0.9 : 0.0;
    
    return hsv_to_rgb(hue, sat, val);
}

inline Uint32 get_color_fire(int iterations, int max_iterations) {
    if (iterations == max_iterations) return 0xFF000000;
    
    double ratio = (double)iterations / max_iterations;
    
    Uint8 r = (Uint8)(fmin(255, ratio * 3 * 255));
    Uint8 g = (Uint8)(fmin(255, ratio * 2 * 255));
    Uint8 b = (Uint8)(ratio * 255);
    
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

inline Uint32 get_color_ocean(int iterations, int max_iterations) {
    if (iterations == max_iterations) return 0xFF000020;
    
    double ratio = (double)iterations / max_iterations;
    double wave = sin(ratio * M_PI * 4) * 0.5 + 0.5;
    
    Uint8 r = (Uint8)(20 + ratio * 30);
    Uint8 g = (Uint8)(50 + ratio * 100 + wave * 50);
    Uint8 b = (Uint8)(150 + ratio * 105);
    
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

inline Uint32 get_color_electric(int iterations, int max_iterations) {
    if (iterations == max_iterations) return 0xFF000000;
    
    double smooth = iterations + 1.0 - log(log(2)) / log(2);
    double hue = fmod(smooth * 8.0 + 180, 360.0);
    double sat = 0.9;
    double val = 0.95;
    
    return hsv_to_rgb(hue, sat, val);
}

inline Uint32 get_color_pastel(int iterations, int max_iterations) {
    if (iterations == max_iterations) return 0xFFFAFAFA;
    
    double ratio = (double)iterations / max_iterations;
    double hue = fmod(ratio * 360.0 + 30, 360.0);
    double sat = 0.3 + ratio * 0.2;
    double val = 0.8 + ratio * 0.2;
    
    return hsv_to_rgb(hue, sat, val);
}

inline Uint32 get_color_monochrome(int iterations, int max_iterations) {
    if (iterations == max_iterations) return 0xFF000000;
    
    Uint8 gray = (Uint8)((double)iterations / max_iterations * 255);
    return (0xFF << 24) | (gray << 16) | (gray << 8) | gray;
}

inline Uint32 get_color(int iterations, int max_iterations, ColorPalette palette) {
    switch (palette) {
        case PALETTE_RAINBOW:   return get_color_rainbow(iterations, max_iterations);
        case PALETTE_FIRE:      return get_color_fire(iterations, max_iterations);
        case PALETTE_OCEAN:     return get_color_ocean(iterations, max_iterations);
        case PALETTE_ELECTRIC:  return get_color_electric(iterations, max_iterations);
        case PALETTE_PASTEL:    return get_color_pastel(iterations, max_iterations);
        case PALETTE_MONOCHROME: return get_color_monochrome(iterations, max_iterations);
        default:                return get_color_rainbow(iterations, max_iterations);
    }
}

// Optimized fractal functions with early bailout
inline int mandelbrot_opt(double real0, double imag0) {
    double real = real0;
    double imag = imag0;
    double realq, imagq;
    int i;

    // Quick check for main cardioid and period-2 bulb
    double q = (real0 - 0.25) * (real0 - 0.25) + imag0 * imag0;
    if (q * (q + (real0 - 0.25)) <= 0.25 * imag0 * imag0) return MAX_ITERATIONS;
    if ((real0 + 1) * (real0 + 1) + imag0 * imag0 <= 0.0625) return MAX_ITERATIONS;

    for (i = 0; i < MAX_ITERATIONS; i++) {
        realq = real * real;
        imagq = imag * imag;
        if (realq + imagq > 4.0) break;
        imag = 2.0 * real * imag + imag0;
        real = realq - imagq + real0;
    }
    return i;
}

inline int julia_opt(double real0, double imag0) {
    double real = real0;
    double imag = imag0;
    double realq, imagq;
    int i;

    for (i = 0; i < MAX_ITERATIONS; i++) {
        realq = real * real;
        imagq = imag * imag;
        if (realq + imagq > 4.0) break;
        imag = 2.0 * real * imag + JULIA_CY;
        real = realq - imagq + JULIA_CX;
    }
    return i;
}

inline int burning_ship_opt(double real0, double imag0) {
    double real = real0;
    double imag = imag0;
    double realq, imagq;
    int i;

    for (i = 0; i < MAX_ITERATIONS; i++) {
        realq = real * real;
        imagq = imag * imag;
        if (realq + imagq > 4.0) break;
        imag = fabs(2.0 * real * imag) + imag0;
        real = realq - imagq + real0;
    }
    return i;
}

inline int calculate_fractal_opt(FractalType type, double real0, double imag0) {
    switch (type) {
        case FRACTAL_MANDELBROT:
            return mandelbrot_opt(real0, imag0);
        case FRACTAL_JULIA:
            return julia_opt(real0, imag0);
        case FRACTAL_BURNING_SHIP:
            return burning_ship_opt(real0, imag0);
        default:
            return 0;
    }
}

void push_history(ZoomState state) {
    zoom_history[history_index] = state;
    history_index = (history_index + 1) % MAX_ZOOM_HISTORY;
    if (history_count < MAX_ZOOM_HISTORY) history_count++;
}

ZoomState pop_history() {
    if (history_count > 0) {
        history_index = (history_index - 1 + MAX_ZOOM_HISTORY) % MAX_ZOOM_HISTORY;
        history_count--;
        return zoom_history[history_index];
    }
    return (ZoomState){-2.0, 0.8, -1.0, 1.0, FRACTAL_MANDELBROT};
}

void screen_to_complex(int screen_x, int screen_y, ZoomState zoom, FractalType type, double *real, double *imag) {
    double width = zoom.realmax - zoom.realmin;
    double height = zoom.imagmax - zoom.imagmin;
    
    *real = zoom.realmin + (screen_x * width) / WIDTH;
    
    if (type == FRACTAL_BURNING_SHIP) {
        *imag = zoom.imagmin + (screen_y * height) / HEIGHT;
    } else {
        *imag = zoom.imagmax - (screen_y * height) / HEIGHT;
    }
}

void* render_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    ZoomState zoom = data->zoom;
    
    for (int y = data->start_y; y < data->end_y; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double r0, i0;
            
            if (data->type == FRACTAL_BURNING_SHIP) {
                r0 = zoom.realmin + x * data->deltareal;
                i0 = zoom.imagmin + y * data->deltaimag;
            } else {
                r0 = zoom.realmin + x * data->deltareal;
                i0 = zoom.imagmax - y * data->deltaimag;
            }
            
            int iterations = calculate_fractal_opt(data->type, r0, i0);
            Uint32 color = get_color(iterations, MAX_ITERATIONS, data->palette);
            data->pixels[y * WIDTH + x] = color;
        }
    }
    
    data->completed = 1;
    return NULL;
}

void render_fractal_color(SDL_Renderer *renderer, SDL_Texture *texture, 
                       FractalType current_fractal, ZoomState current_zoom, 
                       Uint32 *pixels, MouseState *mouse) {
    clock_t start = clock();
    
    double deltareal = (current_zoom.realmax - current_zoom.realmin) / WIDTH;
    double deltaimag = (current_zoom.imagmax - current_zoom.imagmin) / HEIGHT;

    // Clear pixel buffer
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        pixels[i] = 0xFF000000;
    }

    // Create threads for parallel rendering
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int rows_per_thread = HEIGHT / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start_y = i * rows_per_thread;
        thread_data[i].end_y = (i == NUM_THREADS - 1) ? HEIGHT : (i + 1) * rows_per_thread;
        thread_data[i].type = current_fractal;
        thread_data[i].zoom = current_zoom;
        thread_data[i].deltareal = deltareal;
        thread_data[i].deltaimag = deltaimag;
        thread_data[i].pixels = pixels;
        thread_data[i].completed = 0;
        thread_data[i].palette = current_palette;
        
        pthread_create(&threads[i], NULL, render_thread, &thread_data[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Update texture
    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
    
    // Clear renderer and copy texture
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Draw colored border to indicate fractal type
    Uint8 r, g, b;
    switch (current_fractal) {
        case FRACTAL_MANDELBROT:  r = 255; g = 0;   b = 0;   break; // Red
        case FRACTAL_JULIA:       r = 0;   g = 255; b = 0;   break; // Green
        case FRACTAL_BURNING_SHIP: r = 0;   g = 0;   b = 255; break; // Blue
        default:                  r = 128; g = 128; b = 128; break; // Gray
    }
    
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    for (int x = 0; x < WIDTH; x++) {
        SDL_RenderDrawPoint(renderer, x, 0);
        SDL_RenderDrawPoint(renderer, x, 1);
    }
    
    // Draw zoom box if mouse is dragging
    if (mouse->show_zoom_box && mouse->dragging) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
        SDL_Rect zoom_box = {
            .x = (mouse->start_x < mouse->curr_x) ? mouse->start_x : mouse->curr_x,
            .y = (mouse->start_y < mouse->curr_y) ? mouse->start_y : mouse->curr_y,
            .w = abs(mouse->curr_x - mouse->start_x),
            .h = abs(mouse->curr_y - mouse->start_y)
        };
        SDL_RenderDrawRect(renderer, &zoom_box);
        
        // Draw crosshair at center
        int center_x = mouse->start_x + (mouse->curr_x - mouse->start_x) / 2;
        int center_y = mouse->start_y + (mouse->curr_y - mouse->start_y) / 2;
        SDL_RenderDrawLine(renderer, center_x - 5, center_y, center_x + 5, center_y);
        SDL_RenderDrawLine(renderer, center_x, center_y - 5, center_x, center_y + 5);
    }
    
    SDL_RenderPresent(renderer);
    
    clock_t end = clock();
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Calculate zoom level
    double zoom_level = (fractal_params[current_fractal].realmax - fractal_params[current_fractal].realmin) / 
                     (current_zoom.realmax - current_zoom.realmin);
    
    const char* palette_names[] = {"Rainbow", "Fire", "Ocean", "Electric", "Pastel", "Monochrome"};
    printf("Render: %.3fs | Zoom: %.2fx | Palette: %s | History: %d\n", 
           elapsed, zoom_level, palette_names[current_palette], history_count);
}

ZoomState zoom_to_rect(int x1, int y1, int x2, int y2, ZoomState current_zoom) {
    ZoomState new_zoom = current_zoom;
    
    // Ensure proper ordering
    if (x1 > x2) { int temp = x1; x1 = x2; x2 = temp; }
    if (y1 > y2) { int temp = y1; y1 = y2; y2 = temp; }
    
    // Calculate complex coordinates
    double real1, imag1, real2, imag2;
    screen_to_complex(x1, y1, current_zoom, current_zoom.type, &real1, &imag1);
    screen_to_complex(x2, y2, current_zoom, current_zoom.type, &real2, &imag2);
    
    // Set new zoom bounds
    new_zoom.realmin = real1;
    new_zoom.realmax = real2;
    new_zoom.imagmin = imag2;
    new_zoom.imagmax = imag1;
    
    // Check zoom limits
    double width = new_zoom.realmax - new_zoom.realmin;
    if (width < MIN_ZOOM) {
        double center_real = (new_zoom.realmin + new_zoom.realmax) / 2;
        new_zoom.realmin = center_real - MIN_ZOOM / 2;
        new_zoom.realmax = center_real + MIN_ZOOM / 2;
    }
    if (width > MAX_ZOOM) {
        double center_real = (new_zoom.realmin + new_zoom.realmax) / 2;
        new_zoom.realmin = center_real - MAX_ZOOM / 2;
        new_zoom.realmax = center_real + MAX_ZOOM / 2;
    }
    
    return new_zoom;
}

ZoomState zoom_to_point(int x, int y, ZoomState current_zoom, double factor) {
    ZoomState new_zoom = current_zoom;
    
    // Calculate complex coordinates of click point
    double click_real, click_imag;
    screen_to_complex(x, y, current_zoom, current_zoom.type, &click_real, &click_imag);
    
    // Calculate new bounds
    double width = (current_zoom.realmax - current_zoom.realmin) * factor;
    double height = (current_zoom.imagmax - current_zoom.imagmin) * factor;
    
    new_zoom.realmin = click_real - width / 2;
    new_zoom.realmax = click_real + width / 2;
    new_zoom.imagmin = click_imag - height / 2;
    new_zoom.imagmax = click_imag + height / 2;
    
    return new_zoom;
}

void show_instructions() {
    printf("\n=== SDL2 Beautiful Color Fractal Explorer ===\n");
    printf("Keyboard Controls:\n");
    printf("  1-3       - Switch fractal type\n");
    printf("  C          - Cycle color palettes\n");
    printf("  Left Click - Zoom in (draw box to zoom area)\n");
    printf("  Right Click- Zoom in at point (2x)\n");
    printf("  Middle/3rd - Zoom out (2x)\n");
    printf("  Drag       - Pan view\n");
    printf("  R          - Reset to original view\n");
    printf("  Z          - Undo last zoom\n");
    printf("  ESC        - Exit program\n\n");
    printf("Color Palettes:\n");
    printf("  - Rainbow: Smooth HSV color cycling\n");
    printf("  - Fire: Hot red/orange/yellow gradients\n");
    printf("  - Ocean: Cool blue/cyan water effect\n");
    printf("  - Electric: Bright neon-style colors\n");
    printf("  - Pastel: Soft artistic colors\n");
    printf("  - Monochrome: Classic grayscale\n\n");
    printf("Performance:\n");
    printf("  - Multi-threaded rendering (%d threads)\n", NUM_THREADS);
    printf("  - Mathematical color mapping\n");
    printf("  - Smooth gradient interpolation\n");
    printf("  - Interactive zoom and pan\n\n");
}

int main(int argc __attribute__((unused)), char* argv[] __attribute__((unused))) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Beautiful Color Fractal Explorer - Interactive Zoom",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create texture and pixel buffer
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
                                          SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    if (!texture) {
        fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Uint32 *pixels = malloc(WIDTH * HEIGHT * sizeof(Uint32));
    if (!pixels) {
        fprintf(stderr, "Failed to allocate pixel buffer\n");
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    FractalType current_fractal = FRACTAL_MANDELBROT;
    ZoomState current_zoom = {-2.0, 0.8, -1.0, 1.0, FRACTAL_MANDELBROT};
    MouseState mouse = {0};
    
    show_instructions();
    render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT:
                            mouse.dragging = 1;
                            mouse.start_x = e.button.x;
                            mouse.start_y = e.button.y;
                            mouse.curr_x = e.button.x;
                            mouse.curr_y = e.button.y;
                            mouse.show_zoom_box = 1;
                            break;
                            
                        case SDL_BUTTON_RIGHT:
                            push_history(current_zoom);
                            current_zoom = zoom_to_point(e.button.x, e.button.y, current_zoom, 0.5);
                            render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);
                            break;
                            
                        case SDL_BUTTON_MIDDLE:
                            push_history(current_zoom);
                            current_zoom = zoom_to_point(e.button.x, e.button.y, current_zoom, 2.0);
                            render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);
                            break;
                    }
                    break;
                    
                case SDL_MOUSEMOTION:
                    if (mouse.dragging) {
                        mouse.curr_x = e.motion.x;
                        mouse.curr_y = e.motion.y;
                        render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);
                    }
                    break;
                    
                case SDL_MOUSEBUTTONUP:
                    if (e.button.button == SDL_BUTTON_LEFT && mouse.dragging) {
                        if (abs(mouse.curr_x - mouse.start_x) > 5 && abs(mouse.curr_y - mouse.start_y) > 5) {
                            push_history(current_zoom);
                            current_zoom = zoom_to_rect(mouse.start_x, mouse.start_y, 
                                                   mouse.curr_x, mouse.curr_y, current_zoom);
                            render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);
                        }
                        mouse.dragging = 0;
                        mouse.show_zoom_box = 0;
                    }
                    break;
                    
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_1:
                            current_fractal = FRACTAL_MANDELBROT;
                            current_zoom = (ZoomState){-2.0, 0.8, -1.0, 1.0, FRACTAL_MANDELBROT};
                            render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);
                            printf("Switched to: Mandelbrot Set\n");
                            break;
                            
                        case SDLK_2:
                            current_fractal = FRACTAL_JULIA;
                            current_zoom = (ZoomState){-1.7, 1.7, -1.0, 1.0, FRACTAL_JULIA};
                            render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);
                            printf("Switched to: Julia Set\n");
                            break;
                            
                        case SDLK_3:
                            current_fractal = FRACTAL_BURNING_SHIP;
                            current_zoom = (ZoomState){-1.8, -1.7, -0.08, 0.01, FRACTAL_BURNING_SHIP};
                            render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);
                            printf("Switched to: Burning Ship\n");
                            break;
                            
                        case SDLK_c:
                            current_palette = (current_palette + 1) % NUM_PALETTES;
                            render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);
                            printf("Switched palette to: ");
                            const char* palette_names[] = {"Rainbow", "Fire", "Ocean", "Electric", "Pastel", "Monochrome"};
                            printf("%s\n", palette_names[current_palette]);
                            break;
                            
                        case SDLK_r:
                            push_history(current_zoom);
                            switch (current_fractal) {
                                case FRACTAL_MANDELBROT:
                                    current_zoom = (ZoomState){-2.0, 0.8, -1.0, 1.0, FRACTAL_MANDELBROT};
                                    break;
                                case FRACTAL_JULIA:
                                    current_zoom = (ZoomState){-1.7, 1.7, -1.0, 1.0, FRACTAL_JULIA};
                                    break;
                                case FRACTAL_BURNING_SHIP:
                                    current_zoom = (ZoomState){-1.8, -1.7, -0.08, 0.01, FRACTAL_BURNING_SHIP};
                                    break;
                            }
                            render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);
                            printf("Reset view\n");
                            break;
                            
                        case SDLK_z:
                            if (history_count > 0) {
                                current_zoom = pop_history();
                                render_fractal_color(renderer, texture, current_fractal, current_zoom, pixels, &mouse);
                                printf("Undo zoom\n");
                            }
                            break;
                            
                        case SDLK_ESCAPE:
                            quit = 1;
                            break;
                    }
                    break;
            }
        }
        
        SDL_Delay(1);
    }

    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}