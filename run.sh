#!/bin/sh

set -e

# Change to project directory
cd "$(dirname "$0")"

# Build
cmake -B build -S .
cmake --build build

# Run the compiled program with any given arguments
exec ./build/git "$@"
