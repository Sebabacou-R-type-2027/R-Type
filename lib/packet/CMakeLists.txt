add_library(packet)

target_sources(packet
    PRIVATE Packet.cpp
        PacketFactory.cpp
    PUBLIC FILE_SET HEADERS
        FILES Packet.hpp
            PacketFactory.hpp
)

add_subdirectory(packet_type)

find_package(asio CONFIG REQUIRED)

target_link_libraries(packet PRIVATE asio::asio)
