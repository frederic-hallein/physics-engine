# XPBD Softbody Implementation

## Description

**XPBD Softbody Implementation** is a project written in C++ using OpenGL for rendering. The project focuses on simulating softbody dynamics using **Extended Position Based Dynamics (XPBD)** ([Macklin et al., 2019](https://matthias-research.github.io/pages/publications/smallsteps.pdf)), enabling realistic and efficient softbody physics.

### Visuals

![Physics Engine Screenshot](res/screenshots/readme-screenshot.png)

---

## Installation

### Dependencies

- **C++20 Compiler** (e.g., g++ 10+, clang 10+)
- **CMake** (3.10 or newer)
- **OpenGL**
- **GLFW**
- **GLM**
- **ASSIMP**

### Ubuntu/Debian

```sh
sudo apt-get update
sudo apt-get install build-essential cmake libassimp-dev libglm-dev libglfw3-dev libglew-dev
```

### Build Instructions

```sh
git clone https://github.com/frederic-hallein/xpbd-softbody-implementation.git
cd xpbd-softbody-implementation
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./xpbd-softbody
```

---

## Usage

### Controls

- **Left Mouse Button + Drag:** Orbit the camera around the origin.
- **Mouse Scroll Wheel:** Zoom in/out.

#### ImGui Debug Window

- **Reset Camera Button:** Resets camera (shortcut: C).
- **Reset Scene Button:** Resets all objects (shortcut: R).
- **Sliders:** Adjust gravity, alpha, beta, and solver substeps.
- **Toggles:** Enable/disable distance, volume, and collision constraints.

---

## Roadmap

- Improved collision detection and response
- Support for more constraint types
- Enhanced rendering (shadows, materials)
- More object primitives and mesh import formats
- Performance optimizations and parallelization

---

## Project Status

**Not currently in active development.**
Core XPBD simulation and rendering features are implemented.
Additional features and optimizations are planned but not actively worked on.