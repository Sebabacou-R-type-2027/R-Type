#include <iostream>
#include <asio.hpp>

using asio::ip::udp;

int main() {
    try {
        asio::io_context io_context;

        udp::resolver resolver(io_context);
        udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), "localhost", "4242");

        udp::socket socket(io_context);
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
    }

    return 0;
}