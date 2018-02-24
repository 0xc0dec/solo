# TODO Tune for OSX

cmake_minimum_required(VERSION 2.8.4)
project(Solo)

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

add_subdirectory("vendor/assimp/4.1.0")
add_subdirectory("vendor/shaderc")

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Config types" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /MT")
set(ROOT_DIR ${PROJECT_SOURCE_DIR})

# Engine

file(GLOB ENGINE_SRC_BULLET "src/solo/bullet/*.cpp" "src/solo/bullet/*.h")
file(GLOB ENGINE_SRC_GL "src/solo/gl/*.cpp" "src/solo/gl/*.h")
file(GLOB ENGINE_SRC_LUA "src/solo/lua/*.cpp" "src/solo/lua/*.h")
file(GLOB ENGINE_SRC_SDL "src/solo/sdl/*.cpp" "src/solo/sdl/*.h")
file(GLOB ENGINE_SRC_STB "src/solo/stb/*.cpp" "src/solo/stb/*.h")
file(GLOB ENGINE_SRC_VK "src/solo/vk/*.cpp" "src/solo/vk/*.h")
file(GLOB ENGINE_SRC_CORE "src/solo/*.cpp" "src/solo/*.h")

source_group("bullet" FILES ${ENGINE_SRC_BULLET})
source_group("gl" FILES ${ENGINE_SRC_GL})
source_group("lua" FILES ${ENGINE_SRC_LUA})
source_group("sdl" FILES ${ENGINE_SRC_SDL})
source_group("stb" FILES ${ENGINE_SRC_STB})
source_group("vk" FILES ${ENGINE_SRC_VK})
source_group("" FILES ${ENGINE_SRC_CORE})

add_executable(Solo
    ${ENGINE_SRC_BULLET}
    ${ENGINE_SRC_GL}
    ${ENGINE_SRC_LUA}
    ${ENGINE_SRC_SDL}
    ${ENGINE_SRC_STB}
    ${ENGINE_SRC_VK}
    ${ENGINE_SRC_CORE})

target_include_directories(Solo PRIVATE
    "src/solo"
    "vendor" # TODO needed?
    "vendor/glew/1.13/include"
    "vendor/SDL/2.0.4/include"
    "vendor/bullet/2.83.7"
    "vendor/stb_truetype/1.11"
    "vendor/vulkan/include"
    "vendor/LuaIntf"
    "vendor/lua/5.3.1"
    "vendor/stb_image/2.15"
    "vendor/spirv_cross"
    "vendor/glm/0.9.8.4"
    "vendor/stb_image/2.15"
    "vendor/shaderc/libshaderc/include"
    "vendor/assimp/4.1.0/include"
    "${PROJECT_BINARY_DIR}/vendor/assimp/4.1.0/include" # For assimp generated config.h
)

target_link_libraries(Solo Vendor Vendor.Lua)

target_compile_definitions(Solo PRIVATE
    BUILD_WINDOWS
    HAVE_LIBC
    GLEW_STATIC
    SDL_HAPTIC_DISABLED
    SDL_JOYSTICK_DISABLED
)

# Vendor.Lua

file(GLOB_RECURSE LUA_SRC "vendor/lua/5.3.1/*.c")
add_library(Vendor.Lua STATIC ${LUA_SRC})

set_source_files_properties(${LUA_SRC} PROPERTIES LANGUAGE CXX)
target_include_directories(Vendor.Lua PRIVATE "vendor/lua/5.3.1")

# Vendor

file(GLOB GLEW_SRC
    "vendor/glew/1.13/src/glew.c"
)
 
file(GLOB_RECURSE BULLET_SRC
    "vendor/bullet/2.83.7/BulletCollision/*.cpp"
    "vendor/bullet/2.83.7/BulletDynamics/ConstraintSolver/*.cpp"
    "vendor/bullet/2.83.7/BulletDynamics/Dynamics/*.cpp"
    "vendor/bullet/2.83.7/BulletDynamics/MLCPSolvers/*.cpp"
    "vendor/bullet/2.83.7/BulletDynamics/Vehicle/*.cpp"
    "vendor/bullet/2.83.7/LinearMath/*.cpp"
)

file(GLOB SPIRV_CROSS_SRC
    "vendor/spirv_cross/*.cpp"
)

file(GLOB SDL_SRC
    "vendor/SDL/2.0.4/src/haptic/dummy/SDL_syshaptic.c"
    "vendor/SDL/2.0.4/src/joystick/dummy/SDL_sysjoystick.c"
    "vendor/SDL/2.0.4/src/joystick/SDL_gamecontroller.c"
    "vendor/SDL/2.0.4/src/joystick/SDL_joystick.c"
    "vendor/SDL/2.0.4/src/libm/e_atan2.c"
    "vendor/SDL/2.0.4/src/libm/e_log.c"
    "vendor/SDL/2.0.4/src/libm/e_pow.c"
    "vendor/SDL/2.0.4/src/libm/e_rem_pio2.c"
    "vendor/SDL/2.0.4/src/libm/e_sqrt.c"
    "vendor/SDL/2.0.4/src/libm/k_cos.c"
    "vendor/SDL/2.0.4/src/libm/k_rem_pio2.c"
    "vendor/SDL/2.0.4/src/libm/k_sin.c"
    "vendor/SDL/2.0.4/src/libm/k_tan.c"
    "vendor/SDL/2.0.4/src/libm/s_atan.c"
    "vendor/SDL/2.0.4/src/libm/s_copysign.c"
    "vendor/SDL/2.0.4/src/libm/s_cos.c"
    "vendor/SDL/2.0.4/src/libm/s_fabs.c"
    "vendor/SDL/2.0.4/src/libm/s_floor.c"
    "vendor/SDL/2.0.4/src/libm/s_scalbn.c"
    "vendor/SDL/2.0.4/src/libm/s_sin.c"
    "vendor/SDL/2.0.4/src/libm/s_tan.c"
    "vendor/SDL/2.0.4/src/SDL.c"
    "vendor/SDL/2.0.4/src/SDL_assert.c"
    "vendor/SDL/2.0.4/src/atomic/SDL_atomic.c"
    "vendor/SDL/2.0.4/src/audio/SDL_audio.c"
    "vendor/SDL/2.0.4/src/audio/SDL_audiocvt.c"
    "vendor/SDL/2.0.4/src/audio/SDL_audiodev.c"
    "vendor/SDL/2.0.4/src/audio/SDL_audiotypecvt.c"
    "vendor/SDL/2.0.4/src/render/software/SDL_blendfillrect.c"
    "vendor/SDL/2.0.4/src/render/software/SDL_blendline.c"
    "vendor/SDL/2.0.4/src/render/software/SDL_blendpoint.c"
    "vendor/SDL/2.0.4/src/video/SDL_blit.c"
    "vendor/SDL/2.0.4/src/video/SDL_blit_0.c"
    "vendor/SDL/2.0.4/src/video/SDL_blit_1.c"
    "vendor/SDL/2.0.4/src/video/SDL_blit_A.c"
    "vendor/SDL/2.0.4/src/video/SDL_blit_auto.c"
    "vendor/SDL/2.0.4/src/video/SDL_blit_copy.c"
    "vendor/SDL/2.0.4/src/video/SDL_blit_N.c"
    "vendor/SDL/2.0.4/src/video/SDL_blit_slow.c"
    "vendor/SDL/2.0.4/src/video/SDL_bmp.c"
    "vendor/SDL/2.0.4/src/video/SDL_clipboard.c"
    "vendor/SDL/2.0.4/src/events/SDL_clipboardevents.c"
    "vendor/SDL/2.0.4/src/cpuinfo/SDL_cpuinfo.c"
    "vendor/SDL/2.0.4/src/render/SDL_d3dmath.c"
    "vendor/SDL/2.0.4/src/haptic/windows/SDL_dinputhaptic.c"
    "vendor/SDL/2.0.4/src/joystick/windows/SDL_dinputjoystick.c"
    "vendor/SDL/2.0.4/src/audio/directsound/SDL_directsound.c"
    "vendor/SDL/2.0.4/src/audio/disk/SDL_diskaudio.c"
    "vendor/SDL/2.0.4/src/render/software/SDL_drawline.c"
    "vendor/SDL/2.0.4/src/render/software/SDL_drawpoint.c"
    "vendor/SDL/2.0.4/src/events/SDL_dropevents.c"
    "vendor/SDL/2.0.4/src/audio/dummy/SDL_dummyaudio.c"
    "vendor/SDL/2.0.4/src/dynapi/SDL_dynapi.c"
    "vendor/SDL/2.0.4/src/video/SDL_egl.c"
    "vendor/SDL/2.0.4/src/SDL_error.c"
    "vendor/SDL/2.0.4/src/events/SDL_events.c"
    "vendor/SDL/2.0.4/src/video/SDL_fillrect.c"
    "vendor/SDL/2.0.4/src/events/SDL_gesture.c"
    "vendor/SDL/2.0.4/src/stdlib/SDL_getenv.c"
    "vendor/SDL/2.0.4/src/haptic/SDL_haptic.c"
    "vendor/SDL/2.0.4/src/SDL_hints.c"
    "vendor/SDL/2.0.4/src/stdlib/SDL_iconv.c"
    "vendor/SDL/2.0.4/src/events/SDL_keyboard.c"
    "vendor/SDL/2.0.4/src/SDL_log.c"
    "vendor/SDL/2.0.4/src/stdlib/SDL_malloc.c"
    "vendor/SDL/2.0.4/src/audio/SDL_mixer.c"
    "vendor/SDL/2.0.4/src/joystick/windows/SDL_mmjoystick.c"
    "vendor/SDL/2.0.4/src/events/SDL_mouse.c"
    "vendor/SDL/2.0.4/src/video/dummy/SDL_nullevents.c"
    "vendor/SDL/2.0.4/src/video/dummy/SDL_nullframebuffer.c"
    "vendor/SDL/2.0.4/src/video/dummy/SDL_nullvideo.c"
    "vendor/SDL/2.0.4/src/video/SDL_pixels.c"
    "vendor/SDL/2.0.4/src/power/SDL_power.c"
    "vendor/SDL/2.0.4/src/stdlib/SDL_qsort.c"
    "vendor/SDL/2.0.4/src/events/SDL_quit.c"
    "vendor/SDL/2.0.4/src/video/SDL_rect.c"
    "vendor/SDL/2.0.4/src/render/SDL_render.c"
    "vendor/SDL/2.0.4/src/render/direct3d/SDL_render_d3d.c"
    "vendor/SDL/2.0.4/src/render/direct3d11/SDL_render_d3d11.c"
    "vendor/SDL/2.0.4/src/render/opengl/SDL_render_gl.c"
    "vendor/SDL/2.0.4/src/render/opengles2/SDL_render_gles2.c"
    "vendor/SDL/2.0.4/src/render/software/SDL_render_sw.c"
    "vendor/SDL/2.0.4/src/video/SDL_RLEaccel.c"
    "vendor/SDL/2.0.4/src/render/software/SDL_rotate.c"
    "vendor/SDL/2.0.4/src/file/SDL_rwops.c"
    "vendor/SDL/2.0.4/src/render/opengl/SDL_shaders_gl.c"
    "vendor/SDL/2.0.4/src/render/opengles2/SDL_shaders_gles2.c"
    "vendor/SDL/2.0.4/src/video/SDL_shape.c"
    "vendor/SDL/2.0.4/src/atomic/SDL_spinlock.c"
    "vendor/SDL/2.0.4/src/stdlib/SDL_stdlib.c"
    "vendor/SDL/2.0.4/src/video/SDL_stretch.c"
    "vendor/SDL/2.0.4/src/stdlib/SDL_string.c"
    "vendor/SDL/2.0.4/src/video/SDL_surface.c"
    "vendor/SDL/2.0.4/src/thread/generic/SDL_syscond.c"
    "vendor/SDL/2.0.4/src/filesystem/windows/SDL_sysfilesystem.c"
    "vendor/SDL/2.0.4/src/loadso/windows/SDL_sysloadso.c"
    "vendor/SDL/2.0.4/src/thread/windows/SDL_sysmutex.c"
    "vendor/SDL/2.0.4/src/power/windows/SDL_syspower.c"
    "vendor/SDL/2.0.4/src/thread/windows/SDL_syssem.c"
    "vendor/SDL/2.0.4/src/thread/windows/SDL_systhread.c"
    "vendor/SDL/2.0.4/src/timer/windows/SDL_systimer.c"
    "vendor/SDL/2.0.4/src/thread/windows/SDL_systls.c"
    "vendor/SDL/2.0.4/src/thread/SDL_thread.c"
    "vendor/SDL/2.0.4/src/timer/SDL_timer.c"
    "vendor/SDL/2.0.4/src/events/SDL_touch.c"
    "vendor/SDL/2.0.4/src/video/SDL_video.c"
    "vendor/SDL/2.0.4/src/audio/SDL_wave.c"
    "vendor/SDL/2.0.4/src/events/SDL_windowevents.c"
    "vendor/SDL/2.0.4/src/core/windows/SDL_windows.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowsclipboard.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowsevents.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowsframebuffer.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowskeyboard.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowsmessagebox.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowsmodes.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowsmouse.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowsopengl.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowsopengles.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowsshape.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowsvideo.c"
    "vendor/SDL/2.0.4/src/video/windows/SDL_windowswindow.c"
    "vendor/SDL/2.0.4/src/audio/winmm/SDL_winmm.c"
    "vendor/SDL/2.0.4/src/audio/xaudio2/SDL_xaudio2.c"
    "vendor/SDL/2.0.4/src/core/windows/SDL_xinput.c"
    "vendor/SDL/2.0.4/src/render/SDL_yuv_mmx.c"
    "vendor/SDL/2.0.4/src/render/SDL_yuv_sw.c"
)

add_library(Vendor STATIC ${GLEW_SRC} ${BULLET_SRC} ${SPIRV_CROSS_SRC} ${SDL_SRC})

source_group("glew" FILES ${GLEW_SRC})
source_group("bullet" FILES ${BULLET_SRC})
source_group("spirv_cross" FILES ${SPIRV_CROSS_SRC})
source_group("SDL" FILES ${SDL_SRC})

target_include_directories(Vendor PRIVATE
    "vendor"
    "vendor/spirv_cross"
    "vendor/vulkan/include"
    "vendor/glew/1.13/include"
    "vendor/bullet/2.83.7"
    "vendor/SDL/2.0.4/include"
)

find_package(OpenGL REQUIRED)
find_library(VULKAN_LIB
    NAMES vulkan-1
    HINTS "vendor/vulkan/lib"
)
target_link_libraries(Vendor
    ${OPENGL_LIBRARY}
    ${VULKAN_LIB}
    shaderc
    assimp
    zlibstatic
    IrrXML
    winmm.lib
    imm32.lib
    version.lib
)

# TODO Remove copy-paste from same config for Solo
target_compile_definitions(Vendor PRIVATE
    BUILD_WINDOWS # TODO fix for OSX
    HAVE_LIBC
    GLEW_STATIC
    SDL_HAPTIC_DISABLED
    SDL_JOYSTICK_DISABLED
)
