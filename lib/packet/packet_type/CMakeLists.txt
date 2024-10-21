target_sources(packet
    PRIVATE PacketPing.cpp
        PacketACK.cpp
        PacketCMD.cpp
        PacketCMDP.cpp
    PUBLIC FILE_SET HEADERS
        FILES PacketPing.hpp
            PacketACK.hpp
            PacketCMD.hpp
            PacketCMDP.hpp
)

target_include_directories(packet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
