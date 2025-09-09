# 🌌 Solar System Simulation

A real-time simulation of our solar system built with C++, OpenGL, GLSL shaders, and GLFW. This project visualizes planetary motion, orbital dynamics, and lighting effects in a scalable, interactive environment — designed to be both educational and visually compelling.

---

## 🚀 Features

- 🪐 **Planetary Orbits** — Simulates elliptical motion for planets with adjustable speed and radius
- 💡 **Lighting & Shading** — Realistic sunlight using Phong shading and GLSL fragment shaders
- 🛰️ **Camera Navigation** — Fly through space with WASD controls and mouse look
- 🌍 **Scalable Architecture** — Easily add new celestial bodies or tweak orbital parameters
- 🧭 **Time Acceleration** — Speed up or slow down simulation time for dynamic exploration
- 🧵 **Modular Design** — Clean separation of rendering, physics, and UI logic

---

## 🧰 Dependencies

- **GLFW** — Window and input management
- **GLAD** — OpenGL function loader
- **GLM** — Math library for vectors and matrices
- **CMake** — Build system
- **OpenGL 3.3+** — Rendering backend

---

## 🛠️ Build Instructions

### Prerequisites

- C++17-compatible compiler
- CMake ≥ 3.16
- OpenGL drivers installed

### Build Steps

```bash
git clone https://github.com/thelastgoth128/Solar-System.git
cd Solar-System
mkdir build && cd build
cmake ..
cmake --build . --config Release
