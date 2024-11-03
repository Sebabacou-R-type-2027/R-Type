add_executable(r-type_client)

target_sources(r-type_client PRIVATE main.cpp)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(r-type_client PRIVATE game nlohmann_json::nlohmann_json)
