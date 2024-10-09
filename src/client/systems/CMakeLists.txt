file(GLOB sources *.cpp *.hpp)

target_sources(r-type_client PRIVATE ${sources})

add_subdirectory(bullet)
add_subdirectory(button)
add_subdirectory(collision)
add_subdirectory(ennemy_state)
add_subdirectory(loop_movement)
