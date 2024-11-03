add_executable(r-type_server)

target_sources(r-type_server
    PRIVATE main.cpp
        Lobby.cpp
        UdpServer.cpp
    PUBLIC FILE_SET HEADERS
        FILES Lobby.hpp
            UdpServer.hpp
)

add_subdirectory(client)

find_package(asio CONFIG REQUIRED)

target_link_libraries(r-type_server PRIVATE packet asio::asio)
