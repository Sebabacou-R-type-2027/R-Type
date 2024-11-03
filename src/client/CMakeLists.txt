add_executable(r-type_client)

target_sources(r-type_client PRIVATE main.cpp)

target_link_libraries(r-type_client PRIVATE game)
target_link_libraries(r-type_client PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS r-type_client EXPORT main)
