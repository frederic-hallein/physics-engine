# Physics Engine

A 3D physics engine written in C++ using OpenGL for rendering. The goal of this project is to implement **Extended Position Based Dynamics (XPBD)** (See [Macklin, Muller and Chentanez, 2016](https://matthias-research.github.io/pages/publications/XPBD.pdf)) for realistic and efficient physics simulations.


## Features

- **3D Object Rendering**:
  - Load and render `.obj` files with vertex positions, texture coordinates, and normals.

- **Camera System**:
  - Move the camera using keyboard input.
  - Zoom in and out using the scroll wheel.

- **Physics Simulation**:
  - Gravity applied to objects.


## Getting Started

### Prerequisites
---

- **C++ Compiler**: Ensure you have a C++20-compatible compiler installed.
- **CMake**: Used for building the project.
- **OpenGL**: Required for rendering.
- **GLFW**: Required for window and input handling.

### Dependencies
---

This project includes the following dependencies in the `lib/` folder:
- **GLM**: A header-only C++ mathematics library for graphics software.
- **ImGui**: A bloat-free graphical user interface library for C++.

No additional installation is required for these dependencies.

### Installation
---

1. Clone the repository:
```bash
git clone https://github.com/frederic-hallein/physics-engine.git
cd physics-engine
```

2. Create a build directory and configure the project:
```bash
mkdir build
cd build
cmake ..
```

3. Build the project:
```bash
cmake --build .
```

4. Run the executable:
```bash
./physics-engine
```

## Usage

### Controls
---

- **Camera Movement**:
  - `W`: Move forward
  - `S`: Move backward
  - `A`: Move left
  - `D`: Move right
  - `SPACE`: Move up
  - `Shift`: Move down
- **Camera Zoom**:
  - Use the scroll wheel to zoom in and out.


## Screenshots

![Physics Engine Screenshot](res/screenshots/readme-screenshot.png)

