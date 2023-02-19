cmake_minimum_required(VERSION 3.22)

project(catcher)

add_executable(catcher
    main.cpp
    text.cpp
    entities/ball.cpp
    entities/bomb.cpp
    entities/player.cpp
    stage.cpp
    scenes/menu_scene.cpp
    scenes/play_scene.cpp
    scenes/game_over_scene.cpp
    scenes/leaderboard_scene.cpp
)

target_include_directories(catcher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(catcher PRIVATE cxx_std_20)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(catcher PRIVATE box2d::box2d)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(catcher PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(catcher PRIVATE fmt::fmt)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})