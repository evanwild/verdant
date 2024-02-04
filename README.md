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

## Getting Started

*Note: This section is copied from React's docs at react.dev, with JS syntax swapped out for Verdant syntax*

### Creating and nesting components

Verdant apps are made out of components.
A component is a piece of the UI (user interface) that has its own logic and appearance.
A component can be as small as a button, or as large as an entire page.

Verdant components are declared with the `comp` keyword, and contain some HTML markup:

```
comp MyButton() {
  <button>I'm a button</button>
}
```

Now that you’ve declared `MyButton`, you can nest it into another component:

```
comp MyApp() {
  <div>
    <h1>Welcome to my app</h1>
    <MyButton />
  </div>
}
```

Notice that `<MyButton />` starts with a capital letter.
That’s how you know it’s a Verdant component.
Verdant component names must always start with a capital letter, while HTML tags must be lowercase.
