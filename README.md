## WAT?
An engine for my personal graphics/gamedev experiments, not meant for anything serious. Currently works on top of an OpenGL rendering backend, but isn't tightly coupled to it, so another backend might be introduced in future.

## Features
* High-level API exposed to both C++ and Lua.
* Object hierarchies/scene graph.
* Component system: scenes consist of nodes, nodes hold any number of components. Components implement all in-game logic and allow building flexible and modular architecture. Inspired by Unity.
* OBJ meshes and PNG textures loading.
* Very basic resource manager.
* Render to texture.
* Convenient system for applying post-processing effects.

## Screenshots

![1](/screenshots/screenshot3.png?raw=true)
![2](/screenshots/screenshot4.png?raw=true)

## Made possible thanks to
* GLEW
* Lua
* lua-intf
* png
* SDL
* zlib
