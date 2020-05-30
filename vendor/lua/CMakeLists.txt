file(GLOB_RECURSE SL_LUA_SRC "5.3.1/*.c")
add_library(Vendor.Lua STATIC ${SL_LUA_SRC})

set_source_files_properties(${SL_LUA_SRC} PROPERTIES LANGUAGE CXX)
target_include_directories(Vendor.Lua PRIVATE "5.3.1")
