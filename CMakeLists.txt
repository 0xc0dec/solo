# CLion project file

cmake_minimum_required(VERSION 2.8.4)
project(solo)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/lua/5.3.1
    ${PROJECT_SOURCE_DIR}/vendor/LuaIntf
    ${PROJECT_SOURCE_DIR}/vendor/glew/include
    ${PROJECT_SOURCE_DIR}/vendor/SDL/2.0.4/include
    ${PROJECT_SOURCE_DIR}/vendor/png/include
    ${PROJECT_SOURCE_DIR}/vendor/zlib/include)

# Lua

file(GLOB_RECURSE lua_sources "vendor/lua/5.3.1/*.c")
set_source_files_properties(${lua_sources} PROPERTIES LANGUAGE CXX)
add_library(lua STATIC ${lua_sources})

# Engine

file(GLOB_RECURSE sources
    "src/*.cpp"
    "src/platform/*.cpp"
    "src/platform/stub/.cpp"
    "src/platform/opengl/.cpp"
    "src/scripting/lua/*.cpp"
    "vendor/glew/src/*.c"
    "vendor/png/src/*.c"
    "vendor/zlib/src/*.c"
)

file(GLOB sdl_sources
    "vendor/SDL/2.0.4/src/*.c"
    "vendor/SDL/2.0.4/src/atomic/*.c"
    "vendor/SDL/2.0.4/src/audio/*.c"
    "vendor/SDL/2.0.4/src/audio/disk/*.c"
    "vendor/SDL/2.0.4/src/audio/dummy/*.c"
    "vendor/SDL/2.0.4/src/audio/coreaudio/*.c"
    "vendor/SDL/2.0.4/src/cpuinfo/*.c"
    "vendor/SDL/2.0.4/src/dynapi/*.c"
    "vendor/SDL/2.0.4/src/events/*.c"
    "vendor/SDL/2.0.4/src/filesystem/cocoa/*.m"
    "vendor/SDL/2.0.4/src/file/*.c"
    "vendor/SDL/2.0.4/src/file/cocoa/*.m"
    "vendor/SDL/2.0.4/src/haptic/*.c"
    "vendor/SDL/2.0.4/src/haptic/darwin/*.c"
    "vendor/SDL/2.0.4/src/joystick/*.c"
    "vendor/SDL/2.0.4/src/joystick/darwin/*.c"
    "vendor/SDL/2.0.4/src/loadso/dlopen/*.c"
    "vendor/SDL/2.0.4/src/power/*.c"
    "vendor/SDL/2.0.4/src/power/macosx/*.c"
    "vendor/SDL/2.0.4/src/render/*.c"
    "vendor/SDL/2.0.4/src/render/opengl/*.c"
    "vendor/SDL/2.0.4/src/render/software/*.c"
    "vendor/SDL/2.0.4/src/stdlib/*.c"
    "vendor/SDL/2.0.4/src/thread/*.c"
    "vendor/SDL/2.0.4/src/thread/pthread/*.c"
    "vendor/SDL/2.0.4/src/timer/*.c"
    "vendor/SDL/2.0.4/src/timer/unix/*.c"
    "vendor/SDL/2.0.4/src/video/*.c"
    "vendor/SDL/2.0.4/src/video/cocoa/*.m"
    "vendor/SDL/2.0.4/src/video/dummy/*.c"
    "vendor/SDL/2.0.4/src/video/x11/*.c"
)

find_library(carbon_lib Carbon)
find_library(iokit_lib IOKit)
find_library(forcefeedback_lib ForceFeedback)
find_library(cocoa_lib Cocoa)
find_library(audiounit_lib AudioUnit)
find_library(coreaudio_lib CoreAudio)
find_library(corevideo_lib CoreVideo)
find_library(opengl_lib OpenGL)
find_library(corefoundation_lib CoreFoundation)

set(frameworks
    ${carbon_lib}
    ${iokit_lib}
    ${forcefeedback_lib}
    ${cocoa_lib}
    ${audiounit_lib}
    ${coreaudio_lib}
    ${corevideo_lib}
    ${opengl_lib}
    ${corefoundation_lib})

add_library(solo STATIC ${sdl_sources} ${sources})
target_link_libraries(solo lua ${frameworks})

# Tester

file(GLOB_RECURSE tester_sources "tests/tester/*.cpp")
add_executable(Tester ${tester_sources} tests)
target_link_libraries(Tester LINK_PUBLIC solo)
