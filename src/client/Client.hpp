#include <asio.hpp>
#include <thread>
#include <atomic>
#include <iostream>
#include "Packet.hpp"
#include "PacketPing.hpp"

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

            // La boucle principale de logique du client
            void main_loop();

            void send_packet(Packet& packet) const;
            void send_message(const std::string &message);

    protected:
        PacketFactory packet_factory_;
        asio::io_context& io_context_;
        udp::socket socket_;
        udp::endpoint server_endpoint_;
        std::thread receive_thread_;
        std::atomic<bool> is_running_;
        // Méthode de réception des messages du serveur
        void receive_loop();
    };

    class Host : public Client {
        public:
            Host(asio::io_context& io_context, const std::string& server_ip, short server_port)
                : Client(io_context, server_ip, server_port) {}

            // Méthode de gestion de la partie pour l'hôte
            void game_loop();
    };
}

#endif //CLIENT_HPP
