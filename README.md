## WAT?
An engine that I'm planning to use for my graphics/gamedev experiments. Not meant for something serious really.

## What's currently inside
* High-level API exposed to both C++ and Lua.
* Object hierarchies/scene graph.
* Component system: scenes consist of nodes, nodes hold any number of components. Components implement all in-game logic and allow building flexible and modular architecture. Inspired by Unity.
* OBJ meshes and PNG textures loading.
* Very basic resource manager.
* Render to texture.
* Convenient system for applying post-processing effects.

In one picture:
![Current progress](/screenshot2.png?raw=true)

## Made possible thanks to
* GLEW
* Lua
* lua-intf
* png
* SDL
* zlib
