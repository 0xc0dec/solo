# TODO

? (Vulkan) Allocate uniform buffers per material-object pair because
when several changes are made to the shader buffer only the latest is used when render commands are submitted
OR
Build separate command buffers for objects and submit them separately.
OR
Introduce material/mesh binding, store it inside the material, add methods Material::addMeshBinding/removeMeshBinding
and use them for such cases (this is not the first time when I need some sort of mesh-material binding)
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