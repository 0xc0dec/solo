## TODO
* In-memory engine mode (with some stub implementation of all subsystems) in order to be able to test it in a unit fashion
  When this is done Tester would first run engine in an in-memory mode, then in normal mode
* Use ResourceManager in PngTextureLoader like in ObjModelLoader
* More tests for ResourceManager and ModelRenderer
* Play with move semantics in math classes to use memory of a temporary operand to perform calculations there and then move it to the result
