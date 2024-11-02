add_executable(sample)

target_sources(sample
    PRIVATE main.cpp
)

target_link_libraries(sample PRIVATE game)
