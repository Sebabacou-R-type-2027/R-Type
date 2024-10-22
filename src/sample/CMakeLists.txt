add_executable(sample)

target_sources(sample
    PRIVATE main.cpp
)

target_link_libraries(sample PRIVATE ecs)

target_link_libraries(sample PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)
