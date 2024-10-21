file(GLOB sources *.cpp *.hpp)

target_sources(r-type_client PRIVATE ${sources})

add_subdirectory(components)
add_subdirectory(systems)
