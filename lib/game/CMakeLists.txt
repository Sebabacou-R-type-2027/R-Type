add_library(game)

target_sources(game PUBLIC
    FILE_SET CXX_MODULES
        FILES game.cppm
)

target_link_libraries(game PUBLIC ecs sfml-graphics)

add_subdirectory(components)
add_subdirectory(systems)
