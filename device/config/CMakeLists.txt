set(target freertos_config)

add_library(${target} INTERFACE)

target_include_directories(${target} SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${target} INTERFACE projCOVERAGE_TEST=0)