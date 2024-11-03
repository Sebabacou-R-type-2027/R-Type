add_library(game)

target_sources(game PUBLIC
    FILE_SET CXX_MODULES
        FILES module.cppm
            game.cppm
)

add_subdirectory(components)
add_subdirectory(scenes)
add_subdirectory(systems)

find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(game PUBLIC ecs nlohmann_json::nlohmann_json)
