# TODO Tune for OSX

cmake_minimum_required(VERSION 2.8.4)
project(Solo)

option(AMD64 "" OFF)
option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT "" OFF)
option(ASSIMP_BUILD_3DS_IMPORTER "" ON)
option(ASSIMP_BUILD_COLLADA_IMPORTER "" ON)
option(ASSIMP_BUILD_FBX_IMPORTER "" ON)
option(ASSIMP_BUILD_OBJ_IMPORTER "" ON)
option(ASSIMP_BUILD_TESTS "" OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS "" OFF)
option(ASSIMP_NO_EXPORT "" ON)
option(BUILD_GMOCK "" OFF)
option(BUILD_SHARED_LIBS "" OFF)
option(BUILD_TESTING "" OFF)
option(INSTALL_GMOCK "" OFF)
option(INSTALL_GTEST "" OFF)
option(SHADERC_SKIP_INSTALL "" ON)
option(SHADERC_SKIP_TESTS "" ON)
option(SPIRV_CHECK_CONTEXT "" OFF)
option(SPIRV_SKIP_TESTS "" ON)
option(SPIRV_SKIP_EXECUTABLES "" ON)
option(SKIP_GLSLANG_INSTALL "" ON)
option(SKIP_SPIRV_TOOLS_INSTALL "" ON)
option(ENABLE_GLSLANG_BINARIES "" OFF)
option(DISABLE_RTTI "" ON)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Config types" FORCE)

if (MSVC)
    add_compile_options(/std:c++14)
    add_compile_options(/MP)
    add_compile_options("$<$<CONFIG:DEBUG>:/MTd>")
    add_compile_options("$<$<CONFIG:RELEASE>:/MT>")
else()
    add_compile_options(-std=c++14)
endif()

function(set_default_compile_defs TARGET)
    target_compile_definitions(${TARGET} PRIVATE
        HAVE_LIBC
        GLEW_STATIC
        SDL_HAPTIC_DISABLED
        SDL_JOYSTICK_DISABLED
    )
endfunction()

add_subdirectory("vendor")
add_subdirectory("src/demos")
add_subdirectory("src/solr")
add_subdirectory("src/solo")

if (WIN32)
    add_subdirectory("vendor/shaderc")
endif()
