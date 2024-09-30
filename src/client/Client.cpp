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

    void Client::send_packet(Packet& packet) const {
        packet.format_data();
        packet.send_packet(server_endpoint_);
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
            std::cout << "Received: " << std::endl;
            for (auto &byte : buffer) {
                std::cout << static_cast<int>(byte) << "|";
            }
            std::cout << "size of buffer: " << len << std::endl;
            if (std::string(buffer, len) == "ping") {
                std::cout << "Received: ping" << std::endl;
                auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::PING, socket_);
                send_packet(*packet);
            }
            //std::cout << "Received: " << std::string(buffer, len) << std::endl;
        }
    }
}
