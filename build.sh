#!/bin/bash
# Build script for PostLog
# Usage: ./build.sh

set -e

BUILD_DIR="cmake-build-debug"

# Remove old build directory if it exists
if [ -d "$BUILD_DIR" ]; then
  echo "Removing old build directory..."
  rm -rf "$BUILD_DIR"
fi

# Generate build system with CMake and Ninja
cmake -S . -B "$BUILD_DIR" -G Ninja

# Build the project
cmake --build "$BUILD_DIR"

echo "Build complete. Binaries are in $BUILD_DIR."

