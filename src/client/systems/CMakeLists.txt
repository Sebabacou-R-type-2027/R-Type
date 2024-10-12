file(GLOB client *.cpp *.hpp)

set_property(GLOBAL APPEND PROPERTY CLIENT_SOURCES ${client})

add_subdirectory(bullet)
add_subdirectory(button)
add_subdirectory(collision)
add_subdirectory(ennemy_state)
add_subdirectory(loop_movement)
add_subdirectory(shader)
