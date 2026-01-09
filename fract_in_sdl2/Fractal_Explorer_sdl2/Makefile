# Makefile for SDL2 Fractal Explorer
# Combined fractal viewer program
# Combined from veekooFIN's fractal repositories

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
CFLAGS_OPT = -Wall -Wextra -std=c99 -O3 -pthread
LIBS = -lSDL2 -lm
LIBS_OPT = -lSDL2 -lm -lpthread
TARGET = fractal_explorer_sdl2
TARGET_OPT = fractal_explorer_optimized
TARGET_ZOOM = fractal_explorer_zoom
TARGET_COLOR = fractal_explorer_color
SOURCE = fractal_explorer_sdl2.c
SOURCE_OPT = fractal_explorer_optimized.c
SOURCE_ZOOM = fractal_explorer_zoom.c
SOURCE_COLOR = fractal_explorer_color.c

# Default target
all: $(TARGET)

# Build optimized version (default now)
optimized: $(TARGET_OPT)

# Build zoom version
zoom: $(TARGET_ZOOM)

# Build color version
color: $(TARGET_COLOR)

# Build main program
$(TARGET): $(SOURCE)
	@echo "Building SDL2 Fractal Explorer..."
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Build optimized version
$(TARGET_OPT): $(SOURCE_OPT)
	@echo "Building Optimized SDL2 Fractal Explorer..."
	$(CC) $(CFLAGS_OPT) -o $(TARGET_OPT) $(SOURCE_OPT) $(LIBS_OPT)
	@echo "Optimized build complete! Run with: ./$(TARGET_OPT)"

# Build zoom version
$(TARGET_ZOOM): $(SOURCE_ZOOM)
	@echo "Building Interactive Zoom SDL2 Fractal Explorer..."
	$(CC) $(CFLAGS_OPT) -o $(TARGET_ZOOM) $(SOURCE_ZOOM) $(LIBS_OPT)
	@echo "Zoom build complete! Run with: ./$(TARGET_ZOOM)"

# Build color version
$(TARGET_COLOR): $(SOURCE_COLOR)
	@echo "Building Beautiful Color SDL2 Fractal Explorer..."
	$(CC) $(CFLAGS_OPT) -o $(TARGET_COLOR) $(SOURCE_COLOR) $(LIBS_OPT)
	@echo "Color build complete! Run with: ./$(TARGET_COLOR)"

# Build all versions
all: $(TARGET) $(TARGET_OPT) $(TARGET_ZOOM) $(TARGET_COLOR)
	@echo "All versions built successfully!"

# Build both standard and optimized
both: $(TARGET) $(TARGET_OPT)
	@echo "Standard and optimized versions built!"

# Install dependencies (Ubuntu/Debian)
install-deps:
	@echo "Installing SDL2 dependencies..."
	sudo apt update
	sudo apt install -y libsdl2-dev gcc
	@echo "Dependencies installed!"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET) $(TARGET_OPT) $(TARGET_ZOOM) $(TARGET_COLOR)
	@echo "Clean complete!"

# Run the program
run: $(TARGET)
	@echo "Running SDL2 Fractal Explorer..."
	./$(TARGET)

# Run optimized version
run-opt: $(TARGET_OPT)
	@echo "Running Optimized SDL2 Fractal Explorer..."
	./$(TARGET_OPT)

# Run zoom version
run-zoom: $(TARGET_ZOOM)
	@echo "Running Interactive Zoom SDL2 Fractal Explorer..."
	./$(TARGET_ZOOM)

# Run color version
run-color: $(TARGET_COLOR)
	@echo "Running Beautiful Color SDL2 Fractal Explorer..."
	./$(TARGET_COLOR)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)
	@echo "Debug build complete!"

# Debug build for optimized version
debug-opt: CFLAGS_OPT += -g -DDEBUG
debug-opt: $(TARGET_OPT)
	@echo "Debug optimized build complete!"

# Debug build for zoom version
debug-zoom: CFLAGS_OPT += -g -DDEBUG
debug-zoom: $(TARGET_ZOOM)
	@echo "Debug zoom build complete!"

# Debug build for color version
debug-color: CFLAGS_OPT += -g -DDEBUG
debug-color: $(TARGET_COLOR)
	@echo "Debug color build complete!"

# Check for required dependencies
check-deps:
	@echo "Checking for SDL2..."
	@pkg-config --exists sdl2 || (echo "SDL2 not found. Run 'make install-deps'" && exit 1)
	@echo "SDL2 found!"

# Help target
help:
	@echo "SDL2 Fractal Explorer Makefile"
	@echo "=============================="
	@echo ""
	@echo "Available targets:"
	@echo "  all          - Build all versions (default)"
	@echo "  optimized    - Build high-performance version with multi-threading"
	@echo "  zoom         - Build interactive zoom version"
	@echo "  color        - Build beautiful color version (recommended)"
	@echo "  both         - Build standard and optimized versions"
	@echo "  install-deps - Install required dependencies (Ubuntu/Debian)"
	@echo "  check-deps   - Check if dependencies are installed"
	@echo "  run          - Build and run standard program"
	@echo "  run-opt      - Build and run optimized program"
	@echo "  run-zoom     - Build and run interactive zoom program"
	@echo "  run-color    - Build and run beautiful color program"
	@echo "  debug        - Build with debug symbols"
	@echo "  debug-opt    - Build optimized version with debug symbols"
	@echo "  debug-zoom   - Build zoom version with debug symbols"
	@echo "  debug-color  - Build color version with debug symbols"
	@echo "  clean        - Remove build artifacts"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Controls in the program:"
	@echo "  1 - Show Mandelbrot Set"
	@echo "  2 - Show Julia Set"
	@echo "  3 - Show Burning Ship"
	@echo "  ESC - Exit"

# Declare phony targets
.PHONY: all optimized zoom color both install-deps clean run run-opt run-zoom run-color debug debug-opt debug-zoom debug-color check-deps help