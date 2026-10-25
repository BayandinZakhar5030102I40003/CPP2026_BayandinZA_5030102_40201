cmake_minimum_required(VERSION 3.20)
project(Arkanoid)

set(CMAKE_CXX_STANDARD 17)

set(SFML_ROOT "C:/SFML-3.1.0-windows-vc17-64-bit/SFML-3.1.0")

set(SOURCES main.cpp Game.cpp Platform.cpp Ball.cpp Block.cpp Bonus.cpp)

add_executable(Arkanoid ${SOURCES})

target_include_directories(Arkanoid PRIVATE ${SFML_ROOT}/include)
target_link_directories(Arkanoid PRIVATE ${SFML_ROOT}/lib)
target_link_libraries(Arkanoid PRIVATE sfml-graphics sfml-window sfml-system)

add_custom_command(TARGET Arkanoid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_ROOT}/bin/sfml-graphics-3.dll
        ${SFML_ROOT}/bin/sfml-window-3.dll
        ${SFML_ROOT}/bin/sfml-system-3.dll
        $<TARGET_FILE_DIR:Arkanoid>
)