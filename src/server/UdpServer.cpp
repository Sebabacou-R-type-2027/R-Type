#include "UdpServer.hpp"
#include <random>
#include <iostream>
#include <asio.hpp>
#include "client/client.hpp"
#include "client/ClientSaver.hpp"

/**
 * @class UdpServer
 * @brief Implements a UDP server for handling client communication, lobby management, and host selection based on latency.
 */
UdpServer::UdpServer(asio::io_context& io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)), io_context_(io_context) {
    function_map_ = {
        {"create_lobby", [this](const std::string& message) { create_lobby(message); }},
        {"join_lobby", [this](const std::string& message) { join_lobby(message); }},
        {"leave_lobby", [this](const std::string& message) { leave_lobby(message); }},
        {"start_game", [this](const std::string& message) { ping_to_choose_host(remote_endpoint_); }},
        {"logout", [this](const std::string& message) { logout(message); }},
        {"start_game", [this](const std::string& message) { ping_to_choose_host(remote_endpoint_); }},
        {"ping", [this](const std::string& message) { send_ping(remote_endpoint_); }}
    };
    message_id_counter_ = 0;
    receive_thread_ = std::thread(&UdpServer::receive_loop, this);
    server_loop();
}

/**
 * @brief Destructor for UdpServer. Joins the receive thread if it is still joinable.
 */
UdpServer::~UdpServer() {
    std::cout << "Server destructor" << std::endl;
    if (receive_thread_.joinable()) {
        receive_thread_.join();
    }
}

/**
 * @brief Starts asynchronous receiving of messages from clients.
 */
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

/**
 * @brief Main loop for receiving messages. Runs the io_context to handle asynchronous operations.
 */
void UdpServer::receive_loop() {
    start_receive();
    io_context_.run();
}

/**
 * @brief Finds and returns a client based on the provided UDP endpoint.
 *
 * @param client_endpoint The endpoint of the client to retrieve.
 * @return The client associated with the provided endpoint.
 * @throws std::runtime_error if the client is not found.
 */
const server::client UdpServer::get_client(const udp::endpoint &client_endpoint) const{
    for (const auto& cli : connected_clients_) {
        if (cli.get_endpoint() == client_endpoint) {
            return cli;
        }
    }
    throw std::runtime_error("Client not found");
}

/**
 * @brief Creates a new lobby and assigns the client as the host.
 *
 * @param message The message from the client, containing the lobby creation request.
 */
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

/**
 * @brief Allows a client to join an existing lobby.
 *
 * @param message The message containing the lobby ID to join.
 * @throws std::runtime_error if the lobby is not found.
 */
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

/**
 * @brief Allows a client to leave a lobby. Deletes the lobby if the client is the host.
 *
 * @param message The message containing the lobby ID to leave.
 * @throws std::runtime_error if the lobby is not found.
 */
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

/**
 * @brief Handles receiving a message from the socket.
 *
 * @param bytes_transferred The number of bytes transferred in the received message.
 */
void UdpServer::handle_receive(std::size_t bytes_transferred) {
    if (bytes_transferred > 0) {
        std::lock_guard<std::mutex> lock(messages_mutex_);

        std::fill(recv_buffer_.begin() + bytes_transferred, recv_buffer_.end(), 0);
        std::cout << "Received message: " << recv_buffer_.data() << std::endl;

        received_messages_[message_id_counter_] = std::make_pair(recv_buffer_, remote_endpoint_);

        // Increment the message ID
        message_id_counter_ += 1;
        std::cout << "Message ID: " << message_id_counter_ << std::endl;
        // Notify the server loop that a message has been received
        messages_condition_.notify_one();
    }
    start_receive();
}

/**
 * @brief Handles a client message by processing commands or handling client connection state.
 *
 * @param message The message sent by the client.
 * @param client_endpoint The client's endpoint.
 */
void UdpServer::handle_client_message(const std::string& message, const asio::ip::udp::endpoint& client_endpoint) {

    std::string client_str = client_endpoint.address().to_string() + ":" + std::to_string(client_endpoint.port());
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

/**
 * @brief Main server loop. Waits for messages and processes them.
 */
void UdpServer::server_loop() {
    while (true) {
        udp::endpoint client_endpoint;
        std::string message = "";
        {
            std::unique_lock<std::mutex> lock(messages_mutex_);
            // Wait until there are messages to process
            messages_condition_.wait(lock, [this] { return !received_messages_.empty(); });

            // Process the message
            int message_id = received_messages_.begin()->first;
            std::pair<std::array<char, 65535>, udp::endpoint> pair_data = received_messages_.begin()->second;
            message = pair_data.first.data();
            client_endpoint = pair_data.second;

            std::cout << "Received: " << message << " from " << client_endpoint << std::endl;
            received_messages_.erase(message_id);
        }
        // Call the function to handle the client message
        handle_client_message(message, client_endpoint);
    }
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

void UdpServer::send_message(const std::string &message,
                             const udp::endpoint &endpoint) {
    socket_.send_to(asio::buffer(message), endpoint);
}

/**
 * @brief Pings clients in a lobby to measure latency and selects the client with the lowest latency as the game host.
 *
 * @param client_endpoint The endpoint of the client requesting to start the game.
 * @throws std::runtime_error if the client is not the host.
 */
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
        auto start = std::chrono::high_resolution_clock::now();
        {
            std::unique_lock<std::mutex> lock(messages_mutex_);
            messages_condition_.wait(lock, [this]() { return !received_messages_.empty(); });
            if (received_messages_.empty()) {
                continue;
            } else {
                for (const auto& [id, pair_data] : received_messages_) {
                    udp::endpoint sender = pair_data.second;
                    if (static_cast<int>(pair_data.first[2]) == 0 && sender == cli.get_endpoint()) {
                        auto end = std::chrono::high_resolution_clock::now();
                        elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
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

    std::string host = "GAME_LAUNCH|" + cli_tmp.get_ip() + ":" + cli_tmp.get_port() + ":" + std::to_string(cli_tmp.get_id());

    for (auto& cli : clients) {
        if (cli_tmp != cli) {
            std::cout << "Sending " << host<<  " message to: " << cli.get_endpoint() << std::endl;
            send_message(host, cli.get_endpoint());
        } else {
            send_message("GAME_LAUNCH|HOST", cli.get_endpoint());
        }
    }
}

/**
 * @brief Handles a client disconnect by removing the client from the list of authorized clients.
 *
 * @param client_endpoint The endpoint of the client to disconnect.
 */
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

/**
 * @brief Sends a ping message to a client to measure latency.
 *
 * @param client_endpoint The endpoint of the client to ping.
 */
void UdpServer::send_ping(const udp::endpoint& client_endpoint) {
    std::cout << "Sending ping to: " << client_endpoint << std::endl;
    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::PING, socket_);
    packet->format_data();
    packet->send_packet(client_endpoint);
}