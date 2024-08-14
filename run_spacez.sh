#!/bin/bash

# Change to the SpaceZombies directory
cd /Users/michaelzeller/Documents/SpaceZombies

# Remove the existing build directory
rm -r build

# Create a new build directory
mkdir build

# Change to the build directory
cd build

# Run CMake
cmake ..

# Build the project
make

# Run the SpaceZombies executable
./SpaceZombies
