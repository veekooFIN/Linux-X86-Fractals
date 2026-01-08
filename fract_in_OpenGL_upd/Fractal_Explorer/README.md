# Fractal Explorer

A unified fractal exploration program that combines Mandelbrot, Julia, and Burning Ship fractals with interactive zooming capabilities.

## Features

- **Three Fractal Types**: Switch between Mandelbrot, Julia, and Burning Ship fractals
- **Interactive Zooming**: Zoom in/out with mouse controls
- **Adjustable Iterations**: Modify detail level in real-time
- **GPU Accelerated**: Uses OpenGL for fast rendering (~1 second)

## Installation

### Prerequisites

Install the required dependencies on Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install freeglut3-dev build-essential
```

### Compilation

```bash
make
```

Or manually:

```bash
gcc fractal_explorer.c -o fractal_explorer -lGL -lGLU -lglut -lm
```

## Usage

Run the program:

```bash
make run
# or
./fractal_explorer
```

## Controls

| Key/Mouse | Action |
|-----------|--------|
| **1** | Switch to Mandelbrot fractal |
| **2** | Switch to Julia fractal |
| **3** | Switch to Burning Ship fractal |
| **Ctrl + Left Mouse** | Zoom in at cursor position |
| **Right Mouse** | Zoom out from cursor position |
| **+ / =** | Increase max iterations (more detail) |
| **- / _** | Decrease max iterations (less detail) |
| **R** | Reset current fractal view |
| **H** | Show help |
| **ESC** | Exit program |

## Fractal Details

### Mandelbrot Set
- Classic fractal discovered by Benoit Mandelbrot
- Initial view: x ∈ [-2.5, 1.0], y ∈ [-1.4, 1.4]
- Uses standard Mandelbrot iteration: z_{n+1} = z_n^2 + c

### Julia Set
- Related to Mandelbrot set with fixed c parameter
- Initial view: x ∈ [-1.7, 1.7], y ∈ [-1.0, 1.0]
- Uses Julia constant: c = -0.8 + 0.156i
- Iteration: z_{n+1} = z_n^2 + c (where c is fixed)

### Burning Ship
- Variation of Mandelbrot with absolute value operation
- Initial view: x ∈ [-1.8, -1.7], y ∈ [-0.08, 0.01]
- Uses modified iteration: z_{n+1} = z_n^2 + c with |Im(z_n)|

## Performance

- **Rendering time**: ~1 second for 1200x800 resolution
- **Zoom factor**: 0.4 (40% of current view)
- **Default iterations**: 256 (adjustable from 64 to 1024+)

## Color Scheme

The program uses a mathematical color mapping based on iteration count:
- Red: 0.8 + 0.2 × sin(13t + 0.8)
- Green: 0.4 + 0.3 × sin(17t + 2.1)  
- Blue: 0.1 + 0.7 × cos(9t + 1.3)
- Where t = iteration_count / max_iterations

## Technical Details

- **Language**: C
- **Graphics**: OpenGL/GLUT
- **Precision**: Double-precision floating point
- **Window size**: 1200x800 pixels (adjustable)
- **Color depth**: 24-bit RGB

## Cleanup

Remove compiled binary:

```bash
make clean
```

## License

This project is part of the Linux-X86-Fractals collection by veekooFIN.