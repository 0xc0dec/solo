# TODO Tune for OSX

cmake_minimum_required(VERSION 2.8.4)
project(Solo)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Config types" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

