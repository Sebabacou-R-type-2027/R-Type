add_library(game)

target_sources(game PUBLIC
    FILE_SET CXX_MODULES
        FILES module.cppm
            game.cppm
            scenes.cppm
)

target_link_libraries(game PUBLIC ecs sfml-graphics networkClient)

add_subdirectory(components)
add_subdirectory(systems)
