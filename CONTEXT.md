This is a list of notes and TODOs to keep me in context.

# TODO

* Refactor the Lua binding code. It sits in a separate dir and pretends to be replaceable, but
actually the generic engine code relies directly on Lua (see Effect creation from desc files).
* Rename static "create..." funcs into "from..."
* Static mesh collider (demo1 currently has no collision on backdrop mesh)
* Deferred demo - add shadows
* Use GENERAL image layout in Vulkan for now, for simplicity
* Finish cpp demo1
* findScriptComponent to operate on component names
* More error checks
* Different severity levels for error checks
* Fix todos in code