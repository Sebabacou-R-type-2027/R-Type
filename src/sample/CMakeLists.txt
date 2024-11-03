add_executable(sample)

target_sources(sample PRIVATE main.cpp)

target_link_libraries(sample PRIVATE game networkClient asio::asio)

target_include_directories(sample PRIVATE ${ASIO_INCLUDE_DIR})
