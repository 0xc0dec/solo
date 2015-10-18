# CLion project file

cmake_minimum_required(VERSION 2.8.4)
project(solo)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/lua/5.3.1
    ${PROJECT_SOURCE_DIR}/external/LuaIntf
    ${PROJECT_SOURCE_DIR}/external/glew/include
    ${PROJECT_SOURCE_DIR}/external/SDL/2.0.3/include
    ${PROJECT_SOURCE_DIR}/external/png/include
    ${PROJECT_SOURCE_DIR}/external/zlib/include)

link_directories(${PROJECT_SOURCE_DIR}/external/SDL/2.0.3)

# Lua

file(GLOB_RECURSE lua_sources "external/lua/5.3.1/*.c")
set_source_files_properties(${lua_sources} PROPERTIES LANGUAGE CXX)
add_library(lua STATIC ${lua_sources})

# Engine

file(GLOB_RECURSE sources
    "src/*.cpp"
    "src/platform/*.cpp"
    "src/platform/scripting/*.cpp"
    "external/glew/src/*.c"
    "external/png/src/*.c"
    "external/zlib/src/*.c")

find_library(carbon_lib Carbon)
find_library(iokit_lib IOKit)
find_library(forcefeedback_lib ForceFeedback)
find_library(cocoa_lib Cocoa)
find_library(audiounit_lib AudioUnit)
find_library(coreaudio_lib CoreAudio)
find_library(opengl_lib OpenGL)
find_library(corefoundation_lib CoreFoundation)

set(frameworks
    ${carbon_lib}
    ${iokit_lib}
    ${forcefeedback_lib}
    ${cocoa_lib}
    ${audiounit_lib}
    ${coreaudio_lib}
    ${opengl_lib}
    ${corefoundation_lib})

add_library(solo STATIC ${sources})
target_link_libraries(solo lua SDL2 ${frameworks})

# Tester

file(GLOB_RECURSE tester_sources "tests/tester/*.cpp")
add_executable(Tester ${tester_sources} tests)
target_link_libraries(Tester LINK_PUBLIC solo)
