# TODO

* WorldView matrix becomes invalid after a couple of seconds of rotating camera (can be seen in RenderDoc as NaN
values in the uniform variable), this probably causes the bug with crashed while raycasting
* Add no clearing to Vulkan backend and return back other objects rendering in Demo2
* Fix wrong GL textures appeared after switching to always load 4 channel textures
* Continue with Vulkan backend
* Refactor texture/texture data stuff
* Refactor all vulkan code
* More error checks, write errors to console
* Make sure findComponent returns nil for inexistent components.
* Review sptr and maybe replace some of them with raw pointers.
* Fix todos in code