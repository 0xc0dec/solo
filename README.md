## About
"I have no idea how to write game engines"-style game engine. A playground for my gamedev experiments.

## Features
* Unity-inspired component system
* Loading of several popular mesh and texture formats
* TTF fonts
* Bullet physics
* Vulkan/OpenGL rendering
* Same API for both Lua and C++

## Building
* Install Python2 (3 not tested). Needed for building some dependencies.
* `cd build`.
* `cmake -G "Visual Studio 16 2019" ..` (or other generator depending on the OS/IDE).
* Build the generated project.

## Running Lua demos
* `cd build`
* `run-lua-demo1-debug-gl.cmd` or other similar script.

OR

* `cd build/<Debug|Release>`
* `Solr.exe ../../src/demos/lua/demo<N>/demo_<gl|vk>.lua` where `N` is the demo number

## Screenshots

![1](/screenshots/screenshot9.png?raw=true)
![1](/screenshots/screenshot10.png?raw=true)
![1](/screenshots/screenshot11.png?raw=true)
