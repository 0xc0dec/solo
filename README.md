## About
"I have no idea how to write game engines"-style game engine. A playground for my gamedev experiments.

![1](/screenshots/screenshot13.png?raw=true)

## Features
- [Unity](https://unity.com)-inspired component system.
- Loading of several popular mesh and texture formats, also TTF fonts.
- [Bullet](https://github.com/bulletphysics/bullet3) physics.
- Vulkan/OpenGL 4 rendering backend.
- Same API for both Lua and C++.
- [Dear ImGui](https://github.com/ocornut/imgui) UI.

## Building
- Install dependencies:
  - Python2 (Python3 might work, but not tested). Needed for building some engine dependencies.
  - Vulkan SDK.
- `cd build`.
- `cmake -G "Visual Studio 16 2019" -A x64 ..` (or other generator depending on the OS/IDE).
- Build the generated project.

## Running Lua demos

Using scripts
- `cd build`
- `run-lua-demo1-debug-gl.cmd` or other similar script.

or manually
- `cd build/bin/<Debug|Release>`
- `Solr.exe ../../../src/demos/lua/demo<N>/demo_<gl|vk>.lua` where `N` is the demo number

