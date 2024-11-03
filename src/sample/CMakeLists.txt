add_executable(sample)

# find package(nlohmann_json CONFIG REQUIRED)

target_sources(sample
    PRIVATE main.cpp
)


target_link_libraries(sample PRIVATE game)
target_link_libraries(sample PRIVATE nlohmann_json::nlohmann_json)
