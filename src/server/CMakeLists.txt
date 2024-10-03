file(GLOB server *.cpp *.hpp)

add_subdirectory(client)

set_property(GLOBAL APPEND PROPERTY SERVER_SOURCES ${server})
stat