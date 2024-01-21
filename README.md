# Verdant 🌿

Verdant is a work-in-progress language designed for web development. It has three main goals:
- Let people build single-page applications using simple concepts
- Compile to efficient code so end-users have a good experience
- Stay completely separate from the JavaScript ecosystem

## Compiling the Compiler

You must have CMake 3.28+ and a C++ compiler that supports C++20. In the project directory run:

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

This will create a binary called `Verdant` in the build directory.
