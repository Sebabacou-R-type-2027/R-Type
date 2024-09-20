#include <iostream>
#include <asio.hpp>
#include <string>
#include "UdpServer.hpp"

int main(int argc, char* argv[]) {
    try {
        asio::io_context io_context;
        if (argc != 2) {
            std::cerr << "Usage: UdpServer <port>\n";
            return 1;
        }
        UdpServer server(io_context, std::atoi(argv[1])); // Utilise le port 12345
        io_context.run(); // Démarre le service IO
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}