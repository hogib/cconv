# cconv

A simple, dependency-free CLI tool for image manipulation and convolution operations written in pure C.

## Features

### Image Filtering
- **Grayscale** - Convert to single-channel grayscale
- **Invert** - Color inversion
- **Binary Thresholding** - Convert to black and white with configurable threshold
- **Contrast Stretching** - Histogram equalization for contrast enhancement
- **Histogram Equalization** - Improve image contrast distribution

### Convolution Filters
- **Gaussian Blur** - Configurable Gaussian blur (sigma parameter)
- **Laplacian of Gaussian (LoG)** - Edge detection with configurable sigma
- **Sobel X** - Horizontal edge detection
- **Sobel Y** - Vertical edge detection
- **Sobel (Combined)** - Gradient magnitude edge detection

### Core Capabilities
- Sequential filter chaining (apply multiple filters in order)
- Configurable parameters for most filters (sigma, threshold values)
- Verbose logging mode for debugging
- PNG input/output support

## Requirements

- GCC or compatible C compiler (C99 or later)
- Standard C library
- POSIX-compliant system (Linux, macOS, BSD)

## Usage
- cconv [OPTIONS] -i INPUT_FILE [-o OUTPUT_FILE]
