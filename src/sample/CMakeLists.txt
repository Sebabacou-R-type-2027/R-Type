add_executable(sample)

target_sources(sample
    PRIVATE main.cpp
)

add_subdirectory(networkClient)

target_link_libraries(sample PRIVATE game)

target_link_libraries(sample PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)
target_link_libraries(sample PRIVATE asio::asio)
target_link_libraries(sample PRIVATE lz4::lz4)
