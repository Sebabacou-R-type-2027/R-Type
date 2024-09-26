#include <iostream>
#include <asio.hpp>
#include <thread>
#include <atomic>

#ifndef CLIENT_HPP
#define CLIENT_HPP

using asio::ip::udp;

namespace client {
    class Client {
        public:
            Client(asio::io_context& io_context, const std::string& server_ip, short server_port)
                : socket_(io_context, udp::endpoint(udp::v4(), 0)), io_context_(io_context),
                  server_endpoint_(asio::ip::address::from_string(server_ip), server_port),
                  is_running_(true) {
                // Démarrage du thread de réception
                receive_thread_ = std::thread(&Client::receive_loop, this);
            }

            ~Client();
            // Méthode d'envoi d'un message au serveur
            void send_message(const std::string& message);

            // La boucle principale de logique du client
            void main_loop();

        private:
            void receive_loop();

            asio::io_context& io_context_;
            udp::socket socket_;
            udp::endpoint server_endpoint_;
            std::atomic<bool> is_running_;
            std::thread receive_thread_;  // Thread dédié à la réception
        };
}

#endif //CLIENT_HPP
