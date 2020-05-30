file(GLOB SOLR_SRC "./*.cpp" "./*.h")

source_group("" FILES ${SOLR_SRC})

add_executable(Solr ${SOLR_SRC})

target_link_libraries(Solr Solo)

target_include_directories(Solr PRIVATE
    "../../src/solo"
    "../../vendor/glm/0.9.8.4")

if (MSVC)
    target_compile_options(Solr PRIVATE /wd4267 /wd4244 /wd4312)
endif()
