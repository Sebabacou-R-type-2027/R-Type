#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <iostream>
#include <asio.hpp>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include "client/client.hpp"
#include "Lobby.hpp"

using asio::ip::udp;

class UdpServer {
    public:
        UdpServer(asio::io_context &io_context, short port);
        ~UdpServer();
        void server_loop();
        void handle_client_message(const std::string& message, const asio::ip::udp::endpoint& client_endpoint);
        const server::client get_client(const asio::ip::udp::endpoint& client_endpoint) const;
    private:
        void start_receive();
        void receive_loop();
        void handle_receive(std::size_t bytes_transferred);
        void handle_new_connection(const udp::endpoint& client_endpoint);
        void handle_disconnect(const udp::endpoint& client_endpoint);
        void ping_to_choose_host(const udp::endpoint &client_endpoint);
        void create_lobby(const std::string& message);
        void join_lobby(const std::string& message);
        void leave_lobby(std::string message);
        void send_ping(const udp::endpoint& client_endpoint);
        void start_game(const std::string& message);


        asio::io_context& io_context_;
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
        std::vector<server::client> connected_clients_;
        std::map<int, Lobby> lobbies_;
        std::thread receive_thread_;
        std::mutex messages_mutex_;
        std::condition_variable messages_condition_;
        std::unordered_map<int, std::pair<std::string, udp::endpoint>> received_messages_;
        int message_id_counter_ = 0;
        int lobby_id_ = 0;

        std::map<std::string, std::function<void(const std::string&)>> function_map_;
};

#endif //UDPSERVER_HPP
