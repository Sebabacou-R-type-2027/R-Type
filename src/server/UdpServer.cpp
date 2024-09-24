//
// Created by shooting_star_t33 on 9/18/24.
//

#include "UdpServer.hpp"
#include <random>
#include <iostream>
#include <asio.hpp>

UdpServer::UdpServer(asio::io_context& io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)), io_context_(io_context) {
    function_map_ = {
        {"create_lobby", [this](const std::string& message) { create_lobby(message); }},
        {"join_lobby", [this](const std::string& message) { join_lobby(message); }},
        {"leave_lobby", [this](const std::string& message) { leave_lobby(message); }},
        {"start_game", [this](const std::string& message) { ping_to_choose_host(remote_endpoint_); }}
    };
    message_id_counter_ = 0;
    receive_thread_ = std::thread(&UdpServer::receive_loop, this);
    server_loop();
}

UdpServer::~UdpServer() {
    std::cout << "Server destructor" << std::endl;
    if (receive_thread_.joinable()) {
        receive_thread_.join();
    }
}

void UdpServer::start_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                std::cout << "je recois un message" << std::endl;
                handle_receive(bytes_recvd);
            } else {
                start_receive();
            }
        });
}

void UdpServer::receive_loop() {
    start_receive();
    io_context_.run();
}

const server::client UdpServer::get_client(const udp::endpoint &client_endpoint) const{
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

void UdpServer::handle_receive(std::size_t bytes_transferred) {
    std::unique_lock<std::mutex> lock(messages_mutex_);

    if (bytes_transferred > 0) {
        auto message = std::string(recv_buffer_.data(), bytes_transferred);
        std::cout << "Received message: " << message << std::endl;

        received_messages_[message_id_counter_] = std::make_pair(message, remote_endpoint_);

        // Incrémenter l'ID du message
        message_id_counter_ += 1;
        std::cout << "Message ID: " << message_id_counter_ << std::endl;
        // Notifier le serveur loop qu'un message a été reçu
        messages_condition_.notify_one();
    }
    lock.unlock();
    start_receive();
}

void UdpServer::handle_client_message(const std::string& message, const asio::ip::udp::endpoint& client_endpoint) {

    std::string client_str = client_endpoint.address().to_string() + ":" + std::to_string(client_endpoint.port());
    std::cout << "sender: " << client_str << std::endl;
    std::cout << "Authorised clients:" << std::endl;
    for (const auto& client : connected_clients_) {
        std::cout << "\t" << client << std::endl;
    }
    if (message == "login") {
        handle_new_connection(remote_endpoint_);
    } else if (message == "logout") {
        handle_disconnect(remote_endpoint_);
    } else if (message == "start") {
        ping_to_choose_host(remote_endpoint_);
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
}

void UdpServer::server_loop() {
    while (true) {
        std::unique_lock<std::mutex> lock(messages_mutex_);
        std::cout << "Waiting for messages..." << std::endl;
        // Attendre qu'il y ait des messages à traiter
        messages_condition_.wait(lock, [this] { return !received_messages_.empty(); });

        // Traiter le message
        int message_id = received_messages_.begin()->first;
        std::pair<std::string, udp::endpoint> pair_data = received_messages_.begin()->second;
        std::string message = pair_data.first;
        udp::endpoint client_endpoint = pair_data.second;

        std::cout << "Received: " << message << " from " << client_endpoint << std::endl;
        received_messages_.erase(message_id);

        // Appeler la fonction pour gérer le message du client
        handle_client_message(message, client_endpoint);
        lock.unlock();
    }
}

void UdpServer::handle_new_connection(const udp::endpoint& client_endpoint) {
    std::string client_address = client_endpoint.address().to_string();
    std::string client_port = std::to_string(client_endpoint.port());

    auto it = std::find_if(connected_clients_.begin(), connected_clients_.end(), [&client_endpoint](const server::client& cli) {
        return cli.get_id() == std::hash<std::string>{}(client_endpoint.address().to_string() + std::to_string(client_endpoint.port()));
    });

    if (it == connected_clients_.end()) {
        connected_clients_.emplace_back(client_address, client_port, "default_nickname", "default_password");
        std::cout << "New authorised client: " << client_address << ":" << client_port << std::endl;
    } else {
        std::cout << "Client already authorised: " << client_address << ":" << client_port << std::endl;
    }
}

void UdpServer::ping_to_choose_host(const udp::endpoint &client_endpoint) {
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

    std::vector<server::client> clients = lobbies_.at(lobby_id).get_clients();
    long elapsed_time = 0;

    for (auto& cli : clients) {
        send_ping(cli.get_endpoint());
        auto start_time = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < 5) {
            // std::unique_lock<std::mutex> lock(messages_mutex_);
            // messages_condition_.wait_for(lock, std::chrono::seconds(5));
            if (received_messages_.empty()) {
                continue;
            } else {
                for (const auto& [id, pair_data] : received_messages_) {
                    std::string message = pair_data.first;
                    udp::endpoint sender = pair_data.second;
                    if (message == "pong" && sender == cli.get_endpoint()) {
                        auto end_time = std::chrono::steady_clock::now();
                        elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
                        received_messages_.erase(id);
                        break;
                    }
                }
            }
        }
        cli.set_latency(elapsed_time);
        std::cout << "Client latency: " << elapsed_time << std::endl;
        elapsed_time = 0;
    }

    auto cli_tmp = clients[0];

    for (auto& cli : clients) {
        if (cli_tmp != cli) {
            if (cli.get_latency() < cli_tmp.get_latency()) {
                cli_tmp = cli;
            }
        }
    }
    cli_tmp.start_game();
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

void UdpServer::send_ping(const udp::endpoint& client_endpoint) {
    std::string ping_message = "ping";
    socket_.send_to(asio::buffer(ping_message), client_endpoint);
}