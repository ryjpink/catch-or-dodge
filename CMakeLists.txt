cmake_minimum_required(VERSION 3.22)

project(catcher)

add_executable(catcher
    main.cpp
    entities/ball.cpp
    entities/bomb.cpp
    entities/player.cpp
    leaderboard.cpp
    ui/table.cpp
    ui/text.cpp
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

find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
target_link_libraries(catcher PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(catcher PRIVATE cpr::cpr)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(catcher PRIVATE nlohmann_json::nlohmann_json)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(catcher PRIVATE fmt::fmt)

if (APPLE)
    find_library(AUDIO_UNIT_LIBRARY AudioUnit REQUIRED)
    find_library(CORE_AUDIO_LIBRARY CoreAudio REQUIRED)
    target_link_libraries(catcher PRIVATE ${AUDIO_UNIT_LIBRARY} ${CORE_AUDIO_LIBRARY})
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})