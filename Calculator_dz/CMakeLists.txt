cmake_minimum_required(VERSION 3.20)

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/plagins)

add_executable(Calculator_dz
    Calculator.cpp
    Calculator.h
    Command.h
    Parser.cpp
    Parser.h
    Plagins.cpp
    Plagins.h
)

target_include_directories(Calculator_dz PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

if (UNIX AND NOT APPLE)
    target_link_libraries(Calculator_dz PRIVATE ${CMAKE_DL_LIBS})
endif()

function(add_calc_plugin PLUGIN_NAME SOURCE_FILE)
    add_library(${PLUGIN_NAME} SHARED "${SOURCE_FILE}")
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        PREFIX "" 
    )
endfunction()

add_calc_plugin(funcsin "plagins_src/funcsin.cpp")
add_calc_plugin(funcln  "plagins_src/funcln.cpp")
add_calc_plugin(funcexp "plagins_src/funcexp.cpp")

add_dependencies(Calculator_dz funcsin funcln funcexp)

add_custom_command(TARGET Calculator_dz POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Calculator_dz>/plagins
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:funcsin> $<TARGET_FILE_DIR:Calculator_dz>/plagins
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:funcln> $<TARGET_FILE_DIR:Calculator_dz>/plagins
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:funcexp> $<TARGET_FILE_DIR:Calculator_dz>/plagins
)