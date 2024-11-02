file(GLOB sources *.cpp *.hpp)

target_sources(r-type_server PRIVATE ${sources})

add_subdirectory(client)
add_subdirectory(lobby)
add_subdirectory(matchmaking)