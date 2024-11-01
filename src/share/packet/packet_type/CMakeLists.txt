file(GLOB client *.cpp *.hpp)
file(GLOB server *.cpp *.hpp)

set_property(GLOBAL APPEND PROPERTY CLIENT_SOURCES ${client})
set_property(GLOBAL APPEND PROPERTY SERVER_SOURCES ${server})
