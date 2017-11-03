# TODO

* Continue with Vulkan backend
* Handle Vulkan/OpenGL coordinate system differences
(for example add mesh methods setVertices/setUVs/... as in Unity, make these methods fix coordinates;
or add global const vectors storing directions of global axes in relation to the 'screen')
* Refactor texture/texture data stuff
* Refactor all vulkan code
* More error checks, write errors to console
* Make sure findComponent returns nil for inexistent components.
* Review sptr and maybe replace some of them with raw pointers.
* Fix todos in code