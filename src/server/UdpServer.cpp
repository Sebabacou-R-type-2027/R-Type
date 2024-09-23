#include "UdpServer.hpp"
#include <iostream>

UdpServer::UdpServer(asio::io_context &io_context, short port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
    start_receive();
}

void UdpServer::start_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                handle_receive(bytes_recvd);
            } else {
                start_receive();
            }
        });
}

void UdpServer::handle_receive(std::size_t bytes_transferred) {
    const auto message = std::string(recv_buffer_.data(), bytes_transferred);
    handle_client_message(message, remote_endpoint_);
    start_receive();
}

void UdpServer::handle_new_connection(const asio::ip::udp::endpoint &client_endpoint) {
    std::string client_address = client_endpoint.address().to_string();
    std::string client_port = std::to_string(client_endpoint.port());

    auto it = std::find_if(connected_clients_.begin(), connected_clients_.end(), [&client_endpoint](const client &cli) {
        return cli.get_id() == std::hash<std::string>{}(
                   client_endpoint.address().to_string() + std::to_string(client_endpoint.port()));
    });

    if (it == connected_clients_.end()) {
        connected_clients_.emplace_back(client_address, client_port, "default_nickname");
        std::cout << "New authorised client: " << client_address << ":" << client_port << std::endl;
    } else {
        std::cout << "Client already authorised: " << client_address << ":" << client_port << std::endl;
    }
}

client UdpServer::get_client(const asio::ip::udp::endpoint &client_endpoint) {
    const auto it = std::find_if(connected_clients_.begin(), connected_clients_.end(),
                                 [&client_endpoint](const client &cli) {
                                     return cli.get_id() == std::hash<std::string>{}(client_endpoint.address().to_string() + std::to_string(client_endpoint.port()));
                                 });
    if (it != connected_clients_.end()) {
        return *it;
    }
    throw std::runtime_error("Client not found");
}

void UdpServer::handle_client_message(const std::string &message, const asio::ip::udp::endpoint &client_endpoint) {
    int lobby_id = 0;

    if (message == "login") {
        handle_new_connection(client_endpoint);
    } else if (message == "create_lobby") {
        Lobby new_lobby(std::rand(), get_client(client_endpoint));
        new_lobby.add_client(get_client(client_endpoint));
        lobbys_.insert(std::make_pair(new_lobby.get_id(), new_lobby));
        std::cout << "Lobby created: " << new_lobby.get_id() << std::endl;
        std::cout << "Client added to lobby: " << client_endpoint.address().to_string() << ":" << client_endpoint.port()
                << std::endl;
    } else if (std::strstr(message.c_str(), "join_lobby") != nullptr) {
        lobby_id = std::stoi(message.substr(10));
        try {
            lobbys_.at(lobby_id).add_client(get_client(client_endpoint));
            std::cout << "Client joined lobby: " << client_endpoint.address().to_string() << ":" << client_endpoint.
                    port() << std::endl;
        } catch (const std::out_of_range &e) {
            throw std::runtime_error("Lobby not found");
        }
    } else if (message == "start_game") {
        choose_host(client_endpoint);
    }
}

void UdpServer::choose_host(const asio::ip::udp::endpoint &client_endpoint) {
    int lobby_id = -42;
    for (const auto &[id, lobby]: lobbys_) {
        if (lobby.is_host(get_client(client_endpoint))) {
            lobby_id = id;
            break;
        }
    }

    if (lobby_id == -42) {
        throw std::runtime_error("Client is not host");
    }

    lobbys_.at(lobby_id).get_host().start_game();
}
