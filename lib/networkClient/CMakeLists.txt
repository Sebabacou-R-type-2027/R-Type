add_library(networkClient)

#target_sources(networkClient
#        PRIVATE Client.cppm
#            CommandHandler.cppm
#)

target_sources(networkClient PUBLIC
        FILE_SET CXX_MODULES
            FILES Client.cppm
#                CommandHandler.cppm
)

target_include_directories(networkClient
        PUBLIC ${CMAKE_SOURCE_DIR}/lib/packet ${CMAKE_SOURCE_DIR}/lib/packet/packet_type
)

target_link_libraries(networkClient PUBLIC packet asio::asio lz4::lz4)
