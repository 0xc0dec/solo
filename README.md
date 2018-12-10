## About
A playground for my graphics/gamedev experiments.

## Features
* Unity-inspired component system
* Loading of several popular mesh and texture formats
* TTF fonts
* Bullet physics
* Vulkan/OpenGL rendering
* Fully scriptable in Lua

## Building
* `mkdir build && cd build`.
* `cmake -G "Visual Studio 15 2017 Win64" ..` (or other generator depending on the OS/IDE).
* In Visual Studio build solution `build/Solo.sln`. In Clion build the project from CMakeLists file.
* During development: once built, remove from the solution all projects except Solo and use it for development.

## Running
* `cd build/<Debug or Release>`
* For Lua demos: `Solr.exe ../../src/demos/lua/demo<N>_<API>.lua` where `N` is the demo number, `API` is either `gl`
or `vk`

## Screenshots

![1](/screenshots/screenshot9.png?raw=true)
![1](/screenshots/screenshot10.png?raw=true)
![1](/screenshots/screenshot11.png?raw=true)
