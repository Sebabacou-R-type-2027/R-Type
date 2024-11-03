file(GLOB sources *.cpp *.hpp)

add_executable(r-type_server)


target_sources(r-type_server PRIVATE ${sources})


find_package(asio CONFIG REQUIRED)
find_package(lz4 CONFIG REQUIRED)

target_link_libraries(r-type_server PRIVATE packet asio::asio lz4::lz4)

target_include_directories(r-type_server PRIVATE
        client
        lobby
        matchmaking
)

add_subdirectory(client)
add_subdirectory(lobby)
add_subdirectory(matchmaking)

find_package(asio CONFIG REQUIRED)

target_link_libraries(r-type_server PRIVATE packet asio::asio)

install(TARGETS r-type_server EXPORT main
    FILE_SET HEADERS
)
