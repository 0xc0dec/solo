## About
"I have no idea how to write game engines"-style game engine - a playground for my gamedev experiments.
Basically a wrapper around several low-level libraries/APIs in an attempt to make an abstraction on top of them.
These days mostly abandoned and serves as a historical artifact.

![1](/screenshots/screenshot13.png?raw=true)

## Features
- Unity-inspired component system.
- Loading of several popular mesh and texture formats, also TTF fonts.
- Bullet physics.
- Vulkan/OpenGL 4 rendering backend.
- Same-ish API for both Lua and C++.
- Dear ImGui UI.

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

