#include <iostream>
#include <asio.hpp>

using asio::ip::udp;

int main(int argc, char* argv[]) {
    try {
        asio::io_context io_context;
        udp::resolver resolver(io_context);
        udp::socket socket(io_context);
        std::string server_port;
        std::string server_host;
        udp::resolver::results_type endpoints;

        if (argc == 2 || argc == 3) {
            server_port = argv[1];
            try {
                std::stoi(server_port);
            } catch (std::exception& e) {
                throw std::runtime_error("Port must be a number");
            }
            server_host = (argc == 3) ? argv[2] : "localhost";
        } else {
            throw std::runtime_error("Usage: ./r-type_client <port> <host>");
        }

        endpoints = resolver.resolve(udp::v4(), server_host, server_port);
        socket.open(udp::v4());

        std::string message = "";

        while (true) {
            std::cout << "Enter message (exit to quit): ";
            std::getline(std::cin, message);

            if (message == "exit") {
                break;
            }

            socket.send_to(asio::buffer(message), *endpoints.begin());

            std::cout << "Message [" << message << "] send to [" << endpoints.begin()->endpoint() << "]" << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}