file(GLOB sources *.cpp *.hpp)

target_sources(r-type_client PRIVATE ${sources})
target_sources(r-type_server PRIVATE ${sources})

add_subdirectory(packet_type)
