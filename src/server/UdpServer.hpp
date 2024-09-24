#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <iostream>
#include <asio.hpp>
#include <vector>
#include <map>
#include "client/client.hpp"
#include "Lobby.hpp"

using asio::ip::udp;

class UdpServer {
    public:
        UdpServer(asio::io_context& io_context, short port);
        server::client get_client(const asio::ip::udp::endpoint& client_endpoint);
        void handle_new_connection(const udp::endpoint& client_endpoint, const std::string& username, const std::string& password);

    private:
        void start_receive();
        void handle_receive(std::size_t bytes_transferred);
        void handle_disconnect(const udp::endpoint& client_endpoint);
        void choose_host(const udp::endpoint &client_endpoint);
        void create_lobby(const std::string& message);
        void join_lobby(const std::string& message);
        void leave_lobby(std::string message);
        void logout(const std::string& message);

        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
        std::vector<server::client> connected_clients_;
        std::map<int, Lobby> lobbies_;
        int lobby_id_ = 0;

        std::map<std::string, std::function<void(const std::string&)>> function_map_;
};

#endif //UDPSERVER_HPP
