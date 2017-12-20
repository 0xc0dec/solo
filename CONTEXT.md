# TODO

* Fix removeNode method - clears transform first, so other components as transform listeners can't unregister themselves
* Fix post-processor in demo2. Perhaps has something to do with camera render target not being saved in render command,
so if a camera renders twice a frame, with render target and without, only the latest setting is used
* For opengl materials trim out everything before the first dot in parameter names. This will allow use same
calls for settings VK and GL material params.
* Demos refactoring, esp. texture/effect/material loading
* Anisotropy for VK textures
* Use axis names in CubeTextureData for fetching layer data
* Continue with Vulkan backend
* Refactor all vulkan code
* More error checks, write errors to console
* Make sure findComponent returns nil for inexistent components.
* Review sptr and maybe replace some of them with raw pointers.
* Fix todos in code