add_executable(r-type_client)

target_sources(r-type_client PRIVATE main.cpp)

target_link_libraries(r-type_client PRIVATE game)
