file(GLOB SL_ENGINE_SRC_BULLET "bullet/*.cpp" "bullet/*.h")
file(GLOB SL_ENGINE_SRC_GL "gl/*.cpp" "gl/*.h")
file(GLOB SL_ENGINE_SRC_LUA "lua/*.cpp" "lua/*.h")
file(GLOB SL_ENGINE_SRC_SDL "sdl/*.cpp" "sdl/*.h")
file(GLOB SL_ENGINE_SRC_STB "stb/*.cpp" "stb/*.h")
file(GLOB SL_ENGINE_SRC_VK "vk/*.cpp" "vk/*.h")
file(GLOB SL_ENGINE_SRC_MATH "math/*.cpp" "math/*.h")
file(GLOB SL_ENGINE_SRC_CORE "*.cpp" "*.h")

source_group("bullet" FILES ${SL_ENGINE_SRC_BULLET})
source_group("gl" FILES ${SL_ENGINE_SRC_GL})
source_group("lua" FILES ${SL_ENGINE_SRC_LUA})
source_group("sdl" FILES ${SL_ENGINE_SRC_SDL})
source_group("stb" FILES ${SL_ENGINE_SRC_STB})
source_group("vk" FILES ${SL_ENGINE_SRC_VK})
source_group("math" FILES ${SL_ENGINE_SRC_MATH})
source_group("" FILES ${SL_ENGINE_SRC_CORE})

add_library(Solo STATIC
    ${SL_ENGINE_SRC_BULLET}
    ${SL_ENGINE_SRC_GL}
    ${SL_ENGINE_SRC_LUA}
    ${SL_ENGINE_SRC_SDL}
    ${SL_ENGINE_SRC_STB}
    ${SL_ENGINE_SRC_VK}
    ${SL_ENGINE_SRC_MATH}
    ${SL_ENGINE_SRC_CORE})

target_include_directories(Solo PRIVATE
    "."
    "../../vendor" # TODO needed?
    "../../vendor/glew/1.13/include"
    "../../vendor/SDL/2.0.12/include"
    "../../vendor/bullet/2.83.7"
    "../../vendor/stb_truetype/1.11"
    "../../vendor/LuaIntf"
    "../../vendor/lua/5.3.1"
    "../../vendor/stb_image/2.15"
    "../../vendor/spirv_cross"
    "../../vendor/glm/0.9.8.4"
    "../../vendor/stb_image/2.15"
    "../../vendor/shaderc/libshaderc/include"
    "../../vendor/assimp/4.1.0/include"
    "../../vendor/imgui"
    "../../vendor/imgui/examples"
    "${PROJECT_BINARY_DIR}/vendor/assimp/4.1.0/include" # For assimp generated config.h
)

if (NOT APPLE)
    find_package(Vulkan REQUIRED)
    target_include_directories(Solo PRIVATE ${Vulkan_INCLUDE_DIRS} "${Vulkan_INCLUDE_DIRS}/vulkan")
endif()

target_link_libraries(Solo Vendor Vendor.Lua)

if (MSVC)
    target_compile_options(Solo PRIVATE /wd4267 /wd4244 /wd4312)
endif()

set_default_compile_defs(Solo)

target_compile_definitions(Solo PRIVATE "$<$<CONFIG:DEBUG>:SL_DEBUG>")