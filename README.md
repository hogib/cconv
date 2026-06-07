# cconv

A simple, dependency-free CLI tool for image manipulation and convolution operations written in pure C.

## Features

**Image Manipulation:**
- **Invert** - Invert colors in an image
- **Grayscale** - Convert image to grayscale
- **Binary** - Convert image to binary (black and white)

**Convolution Operations:**
- **Gaussian Blur** - Apply Gaussian blur with configurable sigma
- **Sobel X** - Edge detection in X direction
- **Sobel Y** - Edge detection in Y direction

**Additional Features:**
- Chain multiple effects sequentially
- Simple command-line interface
- No external dependencies (uses stb_image for image I/O)
- Supports PNG output
- Verbose debug logging option

