// #include <asio.hpp>
// #include <thread>
// #include <atomic>
// #include <iostream>
// #include "Packet.hpp"
// #include "PacketPing.hpp"

// #ifndef CLIENT_HPP
// #define CLIENT_HPP

// using asio::ip::udp;

// namespace client {
//     class Client {
//         public:
//             Client(asio::io_context& io_context, const std::string& server_ip, short server_port);

//             ~Client();

//             // La boucle principale de logique du client
//             void main_loop();

//             void send_packet(Packet& packet) const;
//             void send_message(const std::string &message);
//             void handle_receive(std::size_t bytes_transferred);

//     protected:
//         PacketFactory packet_factory_;
//         asio::io_context& io_context_;
//         udp::socket socket_;
//         udp::endpoint remote_endpoint_;
//         std::thread receive_thread_;
//         std::atomic<bool> is_running_;
//         std::unordered_map<std::string, udp::endpoint> client_endpoints_;
//         std::mutex messages_mutex_; ///< Mutex pour protéger l'accès aux messages.
//         std::unordered_map<int, std::pair<std::string, udp::endpoint>> received_messages_; ///< Map des messages reçus avec leur ID et leur endpoint.
//         int message_id_counter_ = 0; ///< Compteur d'ID pour les messages.
//         std::condition_variable messages_condition_; ///< Condition variable pour la gestion des messages reçus.
//         std::array<char, 65535> recv_buffer_; ///< Buffer pour stocker les messages reçus.

//         // Méthode de réception des messages du serveur
//         void receive_loop();
//         void start_receive();
//         void manage_message(std::size_t bytes_transferred);
//     };

//     class Host : public Client {
//         public:
//             Host(asio::io_context& io_context, const std::string& server_ip, short server_port)
//                 : Client(io_context, server_ip, server_port) {}

//             // Méthode de gestion de la partie pour l'hôte
//             void game_loop();
//     };
// }

// #endif //CLIENT_HPP
