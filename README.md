## About
"I have no idea how to write game engines"-style game engine. A playground for my gamedev experiments.

## Features
* Unity-inspired component system
* Loading of several popular mesh and texture formats
* TTF fonts
* Bullet physics
* Vulkan/OpenGL rendering
* Exposes API for both Lua and C++

## Building
* Install Python2 (3 not tested).
* `mkdir build && cd build`.
* `cmake -G "Visual Studio 16 2019 Win64" ..` (or other generator depending on the OS/IDE).
* In Visual Studio build solution `build/Solo.sln`. In Clion build the project from CMakeLists file.
* During development: once built, remove from the solution all projects except Solo and use it for development.

## Running Lua demos
* `cd build/<Debug|Release>`
* `Solr.exe ../../src/demos/lua/demo<N>/demo_<gl|vk>.lua` where `N` is the demo number

## Screenshots

![1](/screenshots/screenshot9.png?raw=true)
![1](/screenshots/screenshot10.png?raw=true)
![1](/screenshots/screenshot11.png?raw=true)
