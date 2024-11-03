add_library(game)

target_sources(game PUBLIC
    FILE_SET CXX_MODULES
        FILES module.cppm
            game.cppm
)

target_link_libraries(game PUBLIC ecs)

add_subdirectory(components)
add_subdirectory(scenes)
add_subdirectory(systems)
