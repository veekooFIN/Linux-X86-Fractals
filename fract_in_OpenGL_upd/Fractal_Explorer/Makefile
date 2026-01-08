# Makefile for Fractal Explorer
CC = gcc
CFLAGS = -Wall -O2
LIBS = -lGL -lGLU -lglut -lm

TARGET = fractal_explorer
SOURCE = fractal_explorer.c

# Default target
all: $(TARGET)

# Build the fractal explorer
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y freeglut3-dev build-essential

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean install-deps run