//
// Created by shooting_star_t33 on 9/24/24.
//

#include "Client.hpp"

namespace client {
    Client::~Client() {
        is_running_ = false;  // Arrête la boucle de réception
        if (receive_thread_.joinable()) {
          receive_thread_.join();  // Attends que le thread termine
        }
    }

    void Client::send_message(const std::string &message) {
        socket_.send_to(asio::buffer(message), server_endpoint_);
    }

    void Client::main_loop() {
        std::string input;
        while (is_running_) {
            std::cout << "Enter message to send (or type 'exit' to quit): ";
            std::getline(std::cin, input);

            if (input == "exit") {
                is_running_ = false;
                break;
            }

            send_message(input);
        }
    }

    void Client::receive_loop() {
        while (is_running_) {
            char buffer[1024];
            udp::endpoint sender_endpoint;
            std::error_code error;

            size_t len = socket_.receive_from(asio::buffer(buffer), sender_endpoint, 0, error);

            if (error && error != asio::error::message_size) {
                std::cerr << "Error receiving data: " << error.message() << std::endl;
                return;
            }
            if (std::string(buffer, len) == "ping") {
                std::cout << "Received: ping" << std::endl;
                socket_.send_to(asio::buffer("pong", 4), sender_endpoint);
            }
            //std::cout << "Received: " << std::string(buffer, len) << std::endl;
        }
    }
}
