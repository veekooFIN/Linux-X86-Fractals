# SDL2 Fractal Explorer

A unified fractal viewer that combines multiple fractal rendering programs into one interactive application. This program combines three separate SDL2 fractal programs from veekooFIN's GitHub repositories with two performance versions.

## Features

- **Three Fractal Types**:
  - Mandelbrot Set
  - Julia Set
  - Burning Ship Fractal
- **Four Version Options**:
  - **Color**: Beautiful mathematical palettes (recommended)
  - **Zoom**: Interactive mouse navigation  
  - **Optimized**: High-performance multi-threading
  - **Standard**: Basic keyboard switching
- **Six Color Palettes**: Rainbow, Fire, Ocean, Electric, Pastel, Monochrome
- **Interactive Navigation**: Switch between fractals and palettes in real-time
- **Visual Indicators**: Colored borders and palette name display
- **High Resolution**: 1500x800 pixel display
- **Performance Monitoring**: Real-time rendering time and zoom level display

## Screenshot Preview

```
┌─────────────────────────────────────────────────────────┐
│ == RED BORDER == Mandelbrot Set ======================= │
│                                                         │
│    [Beautiful fractal patterns rendered here]           │
│                                                         │
│    Complex mathematical beauty in grayscale              │
│                                                         │
│ Press 1-3 to switch fractals, ESC to exit             │
└─────────────────────────────────────────────────────────┘
```

## Quick Start

### Prerequisites

```bash
# Ubuntu/Debian systems
sudo apt update
sudo apt install libsdl2-dev gcc

# Or use the Makefile target
make install-deps
```

### Building and Running

```bash
# Clone repository
git clone <repository-url>
cd sdl2

# Build beautiful color version (recommended)
make color

# Run beautiful color fractal explorer
make run-color
# or
./fractal_explorer_color

# Build interactive zoom version (grayscale)
make zoom

# Run interactive zoom fractal explorer
make run-zoom

# Build optimized version (performance without zoom/color)
make optimized

# Build all versions
make all
```

### Version Comparison

| Version | Features | Rendering Time | Interactive |
|---------|-----------|----------------|-------------|
| **Color** | Full mouse zoom + beautiful palettes | ~1.5-2.0 seconds | ✅ Yes | ✅ Beautiful |
| **Zoom** | Full mouse zoom + pan | ~1.5-2.0 seconds | ✅ Yes | ❌ Grayscale |
| Optimized | High-performance multi-threading | ~1.5-2.0 seconds | ❌ No | ❌ Grayscale |
| Standard | Basic keyboard switching | ~5.0 seconds | ❌ No | ❌ Grayscale |

## Controls

### Interactive Zoom Version (Recommended)

#### Mouse Controls
| Action | Button | Effect |
|--------|--------|---------|
| **Zoom to Area** | Left Click + Drag | Draw yellow box, zoom to selected region |
| **Zoom In** | Right Click | 2x zoom at cursor position |
| **Zoom Out** | Middle Click | 2x zoom out from cursor position |

#### Keyboard Controls
| Key | Action |
|-----|--------|
| **1-3** | Switch fractals (maintains current zoom) |
| **C** | Cycle through color palettes |
| **R** | Reset view to original bounds |
| **Z** | Undo last zoom |
| **ESC** | Exit program |
| **Window Close** | Exit program |

#### Visual Indicators
- **Red Border**: Mandelbrot Set
- **Green Border**: Julia Set  
- **Blue Border**: Burning Ship
- **Yellow Box**: Zoom area preview
- **Crosshair**: Zoom center point
- **Color Console**: Current palette name displayed

#### Color Palettes
| Palette | Description | Best For |
|---------|-------------|------------|
| **Rainbow** | Smooth HSV spectrum cycling | Julia spirals, boundaries |
| **Fire** | Hot red→orange→yellow gradients | Burning Ship, hot zones |
| **Ocean** | Cool blue→cyan water effect | Deep zooms, spirals |
| **Electric** | Bright neon-style colors | Complex boundaries |
| **Pastel** | Soft artistic gradients | Delicate patterns |
| **Monochrome** | Classic smooth grayscale | Mathematical purity |

### Standard Version

| Key | Action |
|-----|--------|
| **1** | Show Mandelbrot Set (Red border) |
| **2** | Show Julia Set (Green border) |
| **3** | Show Burning Ship (Blue border) |
| **ESC** | Exit program |
| **Window Close** | Exit program |

## Technical Details

### System Requirements
- Linux-based operating system
- GCC compiler
- SDL2 development libraries
- Graphics display with X11 support

### Performance

#### Standard Version
- **Rendering Time**: ~5 seconds per fractal
- **CPU Usage**: Single core
- **Memory Usage**: ~12MB during rendering

#### Optimized Version
- **Rendering Time**: ~1.5-2.0 seconds per fractal (2.5-3x faster)
- **CPU Usage**: 4 threads (multi-core utilization)
- **Memory Usage**: ~12MB during rendering + thread overhead

#### Common Features
- **Resolution**: 1500x800 pixels
- **Iterations**: 256 per pixel
- **Color Depth**: Grayscale (0-255)

### Algorithm Details

#### Mandelbrot Set
- **Range**: Real [-2.0, 0.8], Imaginary [-1.0, 1.0]
- **Formula**: z(n+1) = z(n)² + c
- **Special Features**: Classic fractal with infinite zoom capability

#### Julia Set
- **Range**: Real [-1.7, 1.7], Imaginary [-1.0, 1.0]
- **Constants**: Cx = -0.8, Cy = 0.156
- **Formula**: z(n+1) = z(n)² + c (with fixed c)
- **Special Features**: Connected symmetric patterns

#### Burning Ship
- **Range**: Real [-1.8, -1.7], Imaginary [-0.08, 0.01]
- **Formula**: z(n+1) = (|Re(z(n))*Im(z(n))|)² + c
- **Special Features**: Uses absolute value for unique "burning" appearance

## Build Options

### Standard Build
```bash
make                # Build optimized version
```

### Debug Build
```bash
make debug          # Build with debug symbols
```

### Clean Build
```bash
make clean          # Remove build artifacts
```

### Check Dependencies
```bash
make check-deps     # Verify SDL2 installation
```

## Project Structure

```
sdl2/
├── fractal_explorer_sdl2.c        # Standard unified program
├── fractal_explorer_optimized.c    # High-performance multi-threaded program
├── fractal_explorer_zoom.c        # Interactive zoom and pan program
├── fractal_explorer_color.c        # Beautiful color version (recommended)
├── fractal_explorer_sdl2          # Standard executable
├── fractal_explorer_optimized      # Optimized executable
├── fractal_explorer_zoom          # Interactive zoom executable
├── fractal_explorer_color          # Beautiful color executable
├── Makefile                      # Build configuration with all targets
├── README.md                     # This file
├── CHANGELOG.md                  # Version history
├── PERFORMANCE.md                # Performance optimization details
└── reference/                    # Original source references
    ├── GigatronTTL-Fractals/
    └── Linux-X86-Fractals/
```

## Source History

### Combined From:

1. **Linux-X86-Fractals/fract_in_sdl2/**
   - `gfxbrot_sdl.c` → Mandelbrot implementation
   - `gfxjulia_sdl.c` → Julia implementation
   - `burnship_sdl.c` → Burning Ship implementation

2. **GigatronTTL-Fractals/**
   - Reference algorithms and optimization techniques
   - Integer vs floating-point performance comparisons

## Troubleshooting

### Common Issues

**SDL2 Not Found**
```bash
# Install SDL2 development libraries
sudo apt install libsdl2-dev

# Check if installed
pkg-config --exists sdl2 && echo "SDL2 found" || echo "SDL2 not found"
```

**Compilation Errors**
```bash
# Ensure GCC is installed
gcc --version

# Check Makefile permissions
chmod +x Makefile
```

**Runtime Issues**
```bash
# Check display server
echo $DISPLAY

# Test SDL2 with simple program
sdl2-config --cflags --libs
```

**Performance Issues**
- Close other applications to free CPU resources
- Ensure adequate RAM (minimum 512MB free)
- Check if hardware acceleration is available

## Development

### Modifying Fractals

To add new fractals or modify existing ones:

1. Add new fractal function to `fractal_explorer_sdl2.c`
2. Update the `FractalType` enum
3. Add parameters to `fractal_params` array
4. Update keyboard input handling

### Optimization Tips

- Use integer arithmetic for faster rendering
- Implement zoom functionality for exploration
- Add color palettes for better visualization
- Consider parallel processing for multi-core systems

## License

This project combines fractal programs from veekooFIN's repositories. Refer to original repositories for specific licensing information.

## Contributing

Feel free to submit issues, enhancement requests, or pull requests. When contributing:

1. Maintain code style consistency
2. Update documentation as needed
3. Test on multiple systems if possible
4. Follow the existing file structure

## Credits

Original fractal implementations by **veekooFIN**:
- [GigatronTTL-Fractals](https://github.com/veekooFIN/GigatronTTL-Fractals)
- [Linux-X86-Fractals](https://github.com/veekooFIN/Linux-X86-Fractals)

Combined into unified explorer with enhanced interactivity.

---

## Version History

See [CHANGELOG.md](CHANGELOG.md) for detailed version history and changes.