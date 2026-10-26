add_executable (Calculator_dz 
    "Calculator.cpp"
    "Calculator.h"
    "Command.h"
    "Parser.h"
    "Parser.cpp"
    "Plagins.h"
    "Plagins.cpp"
)

target_include_directories(Calculator_dz PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

function(add_calc_plugin PLUGIN_NAME SOURCE_FILE)
    add_library(${PLUGIN_NAME} SHARED ${SOURCE_FILE})
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/plagins"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/plagins"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/plagins"
    )
endfunction()

add_calc_plugin(funcsin "plagins_src/funcsin.cpp")
add_calc_plugin(funcln  "plagins_src/funcln.cpp")
add_calc_plugin(funcexp "plagins_src/funcexp.cpp")