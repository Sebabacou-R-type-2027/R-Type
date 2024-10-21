file(GLOB sources *.cpp *.hpp)

target_sources(r-type_client PRIVATE ${sources})

add_subdirectory(networkClient)

add_subdirectory(components)
add_subdirectory(factories)
add_subdirectory(systems)
add_subdirectory(utils)
add_subdirectory(game)
add_subdirectory(multiplayer)
add_subdirectory(menu)
