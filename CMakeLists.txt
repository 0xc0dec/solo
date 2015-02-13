cmake_minimum_required(VERSION 2.8.4)
project(solo)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/glew/include
    ${PROJECT_SOURCE_DIR}/external/SDL/2.0.3/include)

link_directories(${PROJECT_SOURCE_DIR}/external/SDL/2.0.3)

# Engine

file(GLOB_RECURSE sources "src/*.cpp" "external/glew/src/*.c")

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
target_link_libraries(solo SDL2 ${frameworks})

# Tests
add_executable(Test1 tests/test1/Main.cpp)
target_link_libraries(Test1 LINK_PUBLIC solo)