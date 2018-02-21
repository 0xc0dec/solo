# Building shaderc on Windows
* mkdir shaderc/build
* cd shaderc/build
* cmake -G "Visual Studio 15 2017 Win64" ../
* Open and build shaderc/build/libshaderc/libshaderc.sln for appropriate configuration

# Building assimp on Windows
* Generate build files in build/ directory using CMake
* Open sln, fix Runtime Library (dll -> static) for all projects and all configurations
* Build