file(GLOB server *.cpp *.hpp)

add_subdirectory(client)
add_subdirectory(lobby)
add_subdirectory(matchmaking)

set_property(GLOBAL APPEND PROPERTY SERVER_SOURCES ${server})
