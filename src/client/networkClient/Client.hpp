#pragma once
#include "CommandHandler.hpp"
#include "Packet.hpp"
#include "PacketPing.hpp"
#include <asio.hpp>
#include <atomic>
#include <iostream>
#include <thread>

#ifndef CLIENT_HPP
#define CLIENT_HPP

using asio::ip::udp;

namespace client {
    class Client {
        public:
            Client(asio::io_context& io_context, const std::string& server_ip, short server_port);

            ~Client();

            // La boucle principale de logique du client
            void main_loop();

            void send_packet(Packet& packet) const;
            void send_message(const std::string &message);
            void handle_receive(std::size_t bytes_transferred);
            void insertCommands(const std::map<std::string, std::string>& commands);
            void fillCommandsToSends(std::string command);

    private:
        bool im_host = false;
        PacketFactory packet_factory_;
        asio::io_context& io_context_;
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::thread receive_thread_;
        std::thread send_thread_;
        std::atomic<bool> is_running_;
        std::unordered_map<std::string, udp::endpoint> players_endpoints_;
        std::mutex messages_mutex_; ///< Mutex pour protéger l'accès aux messages.
        std::unordered_map<int, std::pair<std::string, udp::endpoint>> received_messages_; ///< Map des messages reçus avec leur ID et leur endpoint.
        int message_id_counter_ = 0; ///< Compteur d'ID pour les messages.
        std::condition_variable messages_condition_; ///< Condition variable pour la gestion des messages reçus.
        std::array<char, 65535> recv_buffer_; ///< Buffer pour stocker les messages reçus.
        int number_of_players_ = 0;
        std::unique_ptr<CommandHandler> command_handler_;
        std::map<std::string, std::string> _commandsToDo;
        std::map<std::string, std::string> _commandsToSend;
        // std::unique_ptr<Host> host_;

        // Méthode de réception des messages du serveur
        void receive_loop();
        void start_receive();
        void manage_message(std::size_t bytes_transferred);
        void send_message_to_player(const std::string& message, const udp::endpoint& player_endpoint);
    };
}

#endif //CLIENT_HPP
