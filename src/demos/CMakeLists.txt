function(add_demo NAME SUBPATH)
    file(GLOB SRC "${SUBPATH}/*.cpp" "${SUBPATH}/*.h")

    source_group("" FILES ${SRC})

    add_executable(${NAME} ${SRC})

    target_link_libraries(${NAME} Solo)

    target_include_directories(${NAME} PRIVATE
        "../../solo"
        "../../vendor/glm/0.9.8.4")

    if (MSVC)
        target_compile_options(${NAME} PRIVATE /wd4267 /wd4244 /wd4312)
    endif()
endfunction()

add_demo(Demo1 demo1)
