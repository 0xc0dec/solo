cmake_minimum_required(VERSION 2.8.4)
project(solo)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")

add_compile_options(-std=c++0x)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/external/SDL/2.0.3/include)

# Engine

file(GLOB sources
    "src/*.h"
    "src/*.cpp"
    "include/*.h")

find_library(carbon_lib Carbon)
find_library(iokit_lib IOKit)
find_library(forcefeedback_lib ForceFeedback)
find_library(cocoa_lib Cocoa)
find_library(audiounit_lib AudioUnit)
find_library(coreaudio_lib CoreAudio)
find_library(opengl_lib OpenGL)
find_library(corefoundation_lib CoreFoundation)

mark_as_advanced(
    carbon_lib
    iokit_lib
    forcefeedback_lib
    cocoa_lib
    audiounit_lib
    coreaudio_lib
    opengl_lib
    corefoundation_lib)

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
link_directories(${PROJECT_SOURCE_DIR}/external/SDL/2.0.3)
target_link_libraries(solo SDL2 ${frameworks})

add_subdirectory("tests/test1")