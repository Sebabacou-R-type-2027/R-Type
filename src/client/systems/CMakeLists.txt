file(GLOB client *.cpp *.hpp)

set_property(GLOBAL APPEND PROPERTY CLIENT_SOURCES ${client})


add_subdirectory(bullet)
add_subdirectory(button)
add_subdirectory(loop_movement)
add_subdirectory(shader)
add_subdirectory(shooting_enemy)
add_subdirectory(power-up)
add_subdirectory(chasing_enemy)
