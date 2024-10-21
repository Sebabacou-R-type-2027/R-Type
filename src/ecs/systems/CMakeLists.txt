file(GLOB sources *.cpp *.hpp)

target_sources(r-type_client PRIVATE ${sources})

add_subdirectory(animation)
add_subdirectory(control)
add_subdirectory(collision)
add_subdirectory(draw)
add_subdirectory(position)
