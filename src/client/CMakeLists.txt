file(GLOB client *.cpp *.hpp )

add_subdirectory(networkClient)

set_property(GLOBAL APPEND PROPERTY CLIENT_SOURCES ${client})
