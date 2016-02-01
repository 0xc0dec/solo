## WAT?
An engine for my personal graphics/gamedev experiments, not meant for anything serious. Currently works ontop of an OpenGL rendering backend, but is built with extensibility in mind.

## Features
* High-level API exposed to both C++ and Lua.
* Object hierarchies/scene graph.
* Component system: scenes consist of nodes, nodes hold any number of components. Components implement all in-game logic and allow building flexible and modular architecture. Inspired by Unity.
* OBJ meshes and PNG textures loading.
* Very basic resource manager.
* Render to texture.
* Convenient system for applying post-processing effects.

In one GIF:

![Current progress](/status.gif?raw=true)

## Made possible thanks to
* GLEW
* Lua
* lua-intf
* png
* SDL
* zlib
