#include <iostream>
#include <asio.hpp>
#include <string>
#include "UdpServer.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: UdpServer <port>\n";
            return 1;
        }

        asio::io_context io_context;
        short port = std::atoi(argv[1]);
        UdpServer server(io_context, port);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}