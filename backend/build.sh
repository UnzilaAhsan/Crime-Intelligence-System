#!/bin/bash

echo "Building Crime Intelligence Backend..."

# Create build directory
mkdir -p build
cd build

# Run CMake
cmake ..

# Compile
make -j4

echo "Build successful!"
echo "Run the backend with: ./build/backend"
