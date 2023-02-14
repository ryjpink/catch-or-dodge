cmake_minimum_required(VERSION 3.22)

project(catcher)

add_executable(catcher
    ball.cpp
    main.cpp
    player.cpp
)

target_compile_features(catcher PRIVATE cxx_std_20)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(catcher PRIVATE box2d::box2d)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(catcher PRIVATE sfml-system sfml-network sfml-graphics sfml-window)