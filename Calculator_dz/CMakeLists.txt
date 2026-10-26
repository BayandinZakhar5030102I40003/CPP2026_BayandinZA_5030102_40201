cmake_minimum_required(VERSION 3.20)

project(Calculator_dz LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

add_executable(Calculator_dz
    Calculator.cpp
    Calculator.h
    Command.h
    Parser.cpp
    Parser.h
    Plagins.cpp
    Plagins.h
)

target_include_directories(Calculator_dz
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

set_target_properties(Calculator_dz PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)

function(add_calc_plugin PLUGIN_NAME SOURCE_FILE)
    add_library(${PLUGIN_NAME} SHARED "${SOURCE_FILE}")

    set_target_properties(${PLUGIN_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/plagins"
        LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/plagins"
    )
endfunction()

add_calc_plugin(funcsin "plagins_src/funcsin.cpp")
add_calc_plugin(funcln  "plagins_src/funcln.cpp")
add_calc_plugin(funcexp "plagins_src/funcexp.cpp")

add_dependencies(Calculator_dz funcsin funcln funcexp)
