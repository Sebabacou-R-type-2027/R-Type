file(GLOB client
        networkClient/*.cpp
        networkClient/*.hpp
        *.cpp
        *.hpp
)
set_property(GLOBAL APPEND PROPERTY CLIENT_SOURCES ${client})
