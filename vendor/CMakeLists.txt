# For spirv/shaderc/assimp
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

# For SDL
option(FORCE_STATIC_VCRT on)

file(GLOB SL_GLEW_SRC "glew/1.13/src/glew.c")
 
file(GLOB_RECURSE SL_BULLET_SRC
    "bullet/2.83.7/BulletCollision/*.cpp"
    "bullet/2.83.7/BulletDynamics/ConstraintSolver/*.cpp"
    "bullet/2.83.7/BulletDynamics/Dynamics/*.cpp"
    "bullet/2.83.7/BulletDynamics/MLCPSolvers/*.cpp"
    "bullet/2.83.7/BulletDynamics/Vehicle/*.cpp"
    "bullet/2.83.7/LinearMath/*.cpp"
)

if (APPLE)
    set(SL_SPIRV_CROSS_SRC "")
else()
    file(GLOB SL_SPIRV_CROSS_SRC "spirv_cross/*.cpp")
endif()


file(GLOB SL_IMGUI_SRC "imgui/*.cpp")
file(GLOB SL_IMGUI_GL_SRC "imgui/examples/imgui_impl_opengl3.cpp")
file(GLOB SL_IMGUI_SDL_SRC "imgui/examples/imgui_impl_sdl.cpp")

if (APPLE)
    file(GLOB SL_IMGUI_VK_SRC "")
else()
    file(GLOB SL_IMGUI_VK_SRC "imgui/examples/imgui_impl_vulkan.cpp")
endif()

add_library(Vendor STATIC
    ${SL_GLEW_SRC}
    ${SL_BULLET_SRC}
    ${SL_SPIRV_CROSS_SRC}
    ${SL_IMGUI_SRC}
    ${SL_IMGUI_SDL_SRC}
    ${SL_IMGUI_GL_SRC}
    ${SL_IMGUI_VK_SRC}
)

source_group("glew" FILES ${SL_GLEW_SRC})
source_group("bullet" FILES ${SL_BULLET_SRC})
source_group("spirv_cross" FILES ${SL_SPIRV_CROSS_SRC})
source_group("imgui" FILES ${SL_IMGUI_SRC} ${SL_IMGUI_SDL_SRC} ${SL_IMGUI_GL_SRC} ${SL_IMGUI_VK_SRC})

target_include_directories(Vendor PRIVATE
    "."
    "spirv_cross"
    "vulkan/include"
    "glew/1.13/include"
    "bullet/2.83.7"
    "SDL/2.0.12/include"
    "imgui"
    "imgui/examples"
)

if (APPLE)
    find_library(SL_CARBON_LIB Carbon)
    find_library(SL_IOKIT_LIB IOKit)
    find_library(SL_FORCEFEEDBACK_LIB ForceFeedback)
    find_library(SL_COCOA_LIB Cocoa)
    find_library(SL_AUDIOUNIT_LIB AudioUnit)
    find_library(SL_COREAUDIO_LIB CoreAudio)
    find_library(SL_COREVIDEO_LIB CoreVideo)
    find_library(SL_COREFOUNDATION_LIB CoreFoundation)

    set(SL_PLATFORM_VENDOR_LIBS
        ${SL_CARBON_LIB}
        ${SL_IOKIT_LIB}
        ${SL_FORCEFEEDBACK_LIB}
        ${SL_COCOA_LIB}
        ${SL_AUDIOUNIT_LIB}
        ${SL_COREAUDIO_LIB}
        ${SL_COREVIDEO_LIB}
        ${SL_COREFOUNDATION_LIB}
    )
else()
    find_library(SL_VULKAN_LIB
        NAMES vulkan-1
        HINTS "vulkan/lib"
    )

    set(SL_PLATFORM_VENDOR_LIBS
        ${SL_VULKAN_LIB}
        shaderc
        zlibstatic
        SDL2-static
        winmm.lib
        imm32.lib
        version.lib
    )
endif()

find_package(OpenGL REQUIRED)

set(SL_VENDOR_LIBS
    ${OPENGL_LIBRARY}
    ${SL_PLATFORM_VENDOR_LIBS}
    assimp
    IrrXML
)

target_link_libraries(Vendor ${SL_VENDOR_LIBS})

set_default_compile_defs(Vendor)

add_subdirectory("lua")
add_subdirectory("assimp/4.1.0")
add_subdirectory("SDL/2.0.12")

if (NOT APPLE)
    add_subdirectory("shaderc")
endif()
