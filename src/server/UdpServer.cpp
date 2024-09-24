#include "UdpServer.hpp"
#include <random>
#include <iostream>
#include <asio.hpp>

#include "client/client.hpp"
#include "client/ClientSaver.hpp"


UdpServer::UdpServer(asio::io_context& io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)) {
    function_map_ = {
        {"create_lobby", [this](const std::string& message) { create_lobby(message); }},
        {"join_lobby", [this](const std::string& message) { join_lobby(message); }},
        {"leave_lobby", [this](const std::string& message) { leave_lobby(message); }},
        {"start_game", [this](const std::string& message) { choose_host(remote_endpoint_); }},
        {"logout", [this](const std::string& message) { logout(message); }}
    };
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

server::client UdpServer::get_client(const udp::endpoint &client_endpoint) {
    for (const auto& cli : connected_clients_) {
        if (cli.get_endpoint() == client_endpoint) {
            return cli;
        }
    }
    throw std::runtime_error("Client not found");
}

void UdpServer::create_lobby(const std::string& message) {
    static std::random_device rd; // Seed for the random number engine
    static std::mt19937 gen(rd()); // Mersenne Twister engine
    static std::uniform_int_distribution<> dis(1, 1000000); // Uniform distribution

    int lobby_id = dis(gen); // Generate a random lobby ID
    Lobby new_lobby(lobby_id, get_client(remote_endpoint_));

    new_lobby.add_client(get_client(remote_endpoint_));
    lobbies_.insert(std::make_pair(new_lobby.get_id(), new_lobby));
    std::cout << "Lobby created: " << new_lobby.get_id() << std::endl;
    std::cout << "Client added to lobby: " << remote_endpoint_.address().to_string() << ":" << remote_endpoint_.port()
            << std::endl;
}

void UdpServer::join_lobby(const std::string& message) {
    const int lobby_id = std::stoi(message.substr(10));

    try {
        lobbies_.at(lobby_id).add_client(get_client(remote_endpoint_));
        std::cout << "Client joined lobby: " << remote_endpoint_.address().to_string() << ":" << remote_endpoint_.
                port() << std::endl;
    } catch (const std::out_of_range &e) {
        throw std::runtime_error("Lobby not found");
    }
}

void UdpServer::leave_lobby(std::string message) {
    const int lobby_id = std::stoi(message.substr(11));

    try {
        auto client = get_client(remote_endpoint_);

        if (client.is_host()) {
            lobbies_.erase(lobby_id);
            std::cout << "Lobby deleted: " << lobby_id << std::endl;
        } else {
            lobbies_.at(lobby_id).remove_client(client);
            std::cout << "Client left lobby: " << remote_endpoint_.address().to_string() << ":" << remote_endpoint_.
                port() << std::endl;
        }
    } catch (const std::out_of_range &e) {
        throw std::runtime_error("Lobby not found");
    }
}

void UdpServer::logout(const std::string& message) {
    handle_disconnect(remote_endpoint_);
}

void UdpServer::handle_receive(std::size_t bytes_transferred) {
    const auto message = std::string(recv_buffer_.data(), bytes_transferred);
    std::string client_str = remote_endpoint_.address().to_string() + ":" + std::to_string(remote_endpoint_.port());

    std::cout << "sender: " << client_str << std::endl;
    std::cout << "Authorised clients:" << std::endl;
    for (const auto& client : connected_clients_) {
        std::cout << "\t" << client << std::endl;
    }
    if (message.rfind("login ", 0) == 0) {
        std::string username, password;
        std::istringstream iss(message.substr(6));

        std::getline(iss, username, ' ');
        std::getline(iss, password);

        if (!username.empty() && !password.empty() && password.find(' ') == std::string::npos) {
            handle_new_connection(remote_endpoint_, username, password);
        } else {
            std::cout << "Invalid login format" << std::endl;
        }
    } else {
        auto it = std::find_if(connected_clients_.begin(), connected_clients_.end(), [&client_str](const server::client& cli) {
            return cli.get_ip() + ":" + cli.get_port() == client_str;
        });
        if (it != connected_clients_.end()) {
            std::cout << "Received: " << message << " from: " << client_str << std::endl;
            bool command_found = false;
            for (const auto& [command, func] : function_map_) {
                if (message.find(command) == 0) { // Check if the message starts with the command
                    func(message);
                    command_found = true;
                    break;
                }
            }
            if (!command_found) {
                std::cout << "Unknown command: " << message << std::endl;
            }
        } else {
            std::cout << "Not login client: " << client_str << std::endl;
        }
    }

    start_receive();
}

void UdpServer::handle_new_connection(const udp::endpoint& client_endpoint, const std::string& username, const std::string& password) {
    std::string client_address = client_endpoint.address().to_string();
    std::string client_port = std::to_string(client_endpoint.port());
    server::ClientSaver cs("clients.csv");

    auto it = std::find_if(connected_clients_.begin(), connected_clients_.end(), [&client_endpoint](const server::client& cli) {
        return cli.get_id() == std::hash<std::string>{}(client_endpoint.address().to_string() + std::to_string(client_endpoint.port()));
    });

    try {
        uint32_t id = cs.check_if_user_already_exists_in_db(username, password);
        if (id) {
            try {
                connected_clients_.emplace_back(client_address, client_port, username, password, id);
                std::cout << "New authorised client from db: " << client_address << ":" << client_port << std::endl;
            } catch (const server::client::ClientException& e) {
                std::cout << e.what() << std::endl;
            }
        } else if (it == connected_clients_.end()) {
            connected_clients_.emplace_back(client_address, client_port, username, password);
            std::cout << "New authorised client: " << client_address << ":" << client_port << std::endl;
            cs.save_client(connected_clients_.back());
        } else {
            std::cout << "Client already authorised: " << client_address << ":" << client_port << std::endl;
        }
    } catch (const server::ClientSaver::ClientSaverException& e) {
        std::cerr << e.what() << std::endl;
    } catch (const server::client::ClientException& e) {
        std::cout << e.what() << std::endl;
        std::cout << "Password or Username is not correct" << std::endl;
    }
}

void UdpServer::choose_host(const udp::endpoint &client_endpoint) {
    int lobby_id = -42;
    for (const auto &[id, lobby]: lobbies_) {
        if (lobby.is_host(get_client(client_endpoint))) {
            lobby_id = id;
            break;
        }
    }

    if (lobby_id == -42) {
        throw std::runtime_error("Client is not host");
    }

    lobbies_.at(lobby_id).get_host().start_game();
}

void UdpServer::handle_disconnect(const udp::endpoint& client_endpoint) {
    std::string client_address = client_endpoint.address().to_string();
    std::string client_port = std::to_string(client_endpoint.port());

    auto it = std::remove_if(connected_clients_.begin(), connected_clients_.end(), [&client_address, &client_port](const server::client& cli) {
        return cli.get_ip() == client_address && cli.get_port() == client_port;
    });

    if (it != connected_clients_.end()) {
        connected_clients_.erase(it, connected_clients_.end());
        std::cout << "Client disconnected: " << client_address << ":" << client_port << std::endl;
    } else {
        std::cout << "Client not found: " << client_address << ":" << client_port << std::endl;
    }
}