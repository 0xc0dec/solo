# CLion project file

cmake_minimum_required(VERSION 2.8.4)
project(solo)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/glew/1.13/include
    ${PROJECT_SOURCE_DIR}/vendor/SDL/2.0.4/include
    ${PROJECT_SOURCE_DIR}/vendor/png/1.6.16/include
    ${PROJECT_SOURCE_DIR}/vendor/zlib/1.2.8/include
    ${PROJECT_SOURCE_DIR}/vendor/async++/include
    ${PROJECT_SOURCE_DIR}/vendor/bullet/2.83.7
    ${PROJECT_SOURCE_DIR}/vendor/stb/truetype/1.11
)

# Engine

file(GLOB_RECURSE sources
    "src/solo/*.cpp"
    "src/solo/platform/null/.cpp"
    "src/solo/platform/sdl/.cpp"
    "src/solo/platform/opengl/.cpp"
    "src/solo/platform/bullet/.cpp"
)

file(GLOB_RECURSE glew_sources "vendor/glew/1.13/src/*.c")
file(GLOB_RECURSE png_sources "vendor/png/1.6.16/src/*.c")
file(GLOB_RECURSE zlib_sources "vendor/zlib/1.2.8/src/*.c")
file(GLOB_RECURSE asyncpp_sources "vendor/async++/src/*.cpp")
file(GLOB_RECURSE bullet_sources
    "vendor/bullet/2.83.7/BulletCollision/*.cpp"
    "vendor/bullet/2.83.7/BulletDynamics/ConstraintSolver/*.cpp"
    "vendor/bullet/2.83.7/BulletDynamics/Dynamics/*.cpp"
    "vendor/bullet/2.83.7/BulletDynamics/MLCPSolvers/*.cpp"
    "vendor/bullet/2.83.7/BulletDynamics/Vehicle/*.cpp"
    "vendor/bullet/2.83.7/LinearMath/*.cpp"
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

add_library(solo STATIC
        ${sdl_sources}
        ${glew_sources}
        ${png_sources}
        ${zlib_sources}
        ${asyncpp_sources}
        ${bullet_sources}
        ${sources})
target_link_libraries(solo ${frameworks})

# Tests

file(GLOB_RECURSE tester_sources "src/tests/tester/*.cpp")
add_executable(Tester ${tester_sources})
target_link_libraries(Tester LINK_PUBLIC solo)

# Demos

file(GLOB_RECURSE demo1_sources "src/demos/demo1/*.cpp")
add_executable(Demo1 ${demo1_sources})
target_link_libraries(Demo1 LINK_PUBLIC solo)

file(GLOB_RECURSE demo2_sources "src/demos/demo2/*.cpp")
add_executable(Demo2 ${demo2_sources})
target_link_libraries(Demo2 LINK_PUBLIC solo)

file(GLOB_RECURSE demo3_sources "src/demos/demo3/*.cpp")
add_executable(Demo3 ${demo3_sources})
target_link_libraries(Demo3 LINK_PUBLIC solo)

file(GLOB_RECURSE demo4_sources "src/demos/demo4/*.cpp")
add_executable(Demo4 ${demo4_sources})
target_link_libraries(Demo4 LINK_PUBLIC solo)
