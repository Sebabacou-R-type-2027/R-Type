#include "UdpServer.hpp"
#include <random>
#include <iostream>
#include <asio.hpp>
#include <Packet.hpp>
#include <PacketACK.hpp>
#include <PacketCMD.hpp>
#include <PacketFactory.hpp>
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
        {"logout", [this](const std::string& message) { logout(message); }},
      	{"login", [this](const std::string& message) { handle_login(message); }},
      	{"lobby_info", [this](const std::string& message) { send_lobby_info(message); }},
      	{"matchmaking", [this](const std::string& message) { add_client_to_matchmaking(message); }},
        {"score", [this](const std::string& message) { get_best_score_cli(message); }},
        {"new_score", [this](const std::string& message) {  new_score_cli(message); }}
    };
    message_id_counter_ = 0;
    receive_thread_ = std::thread(&UdpServer::receive_loop, this);
    matchmaking_system_ = MatchmakingSystem();
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
server::client& UdpServer::get_client(const udp::endpoint &client_endpoint) {
    for (auto& client : connected_clients_) {
        if (client.get_endpoint() == client_endpoint) {
            return client;
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
    auto &client = get_client(remote_endpoint_);
    client.set_lobby_id(lobby_id);
    lobbies_.insert(std::make_pair(new_lobby.get_id(), new_lobby));
    std::cout << "Lobby created: " << new_lobby.get_id() << std::endl;
    std::cout << "Client added to lobby: " << remote_endpoint_.address().to_string() << ":" << remote_endpoint_.port()
            << std::endl;
    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMD, socket_);
    if (typeid(*packet) == typeid(PacketCMD))
    {
        dynamic_cast<PacketCMD*>(packet.get())->format_data(std::to_string(new_lobby.get_id()));
    }
    packet->send_packet(remote_endpoint_);
}

/**
 * @brief Allows a client to join an existing lobby.
 *
 * @param message The message containing the lobby ID to join.
 * @throws std::runtime_error if the lobby is not found.
 */
void UdpServer::join_lobby(const std::string& message) {
    const int lobby_id = std::stoi(message.substr(10));
	auto &client = get_client(remote_endpoint_);
    try {
        lobbies_.at(lobby_id).add_client(get_client(remote_endpoint_));
        std::cout << "Client joined lobby: " << remote_endpoint_.address().to_string() << ":" << remote_endpoint_.
                port() << "lobby id:" << lobby_id <<std::endl;
        client.set_lobby_id(lobby_id);
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

        received_messages_[message_id_counter_] = std::make_pair(recv_buffer_, remote_endpoint_);
        recv_buffer_size_ = bytes_transferred;
        // Increment the message ID
        message_id_counter_ += 1;
        // std::cout << "Message ID: " << message_id_counter_ << std::endl;
        // Notify the server loop that a message has been received
        messages_condition_.notify_one();
    }
    start_receive();
}

/**
* @brief add a client to the matchmaking queue
* @param client_endpoint The endpoint of the client to add to the matchmaking queue
*/
void UdpServer::add_client_to_matchmaking(const std::string& message) {
  	const udp::endpoint& client_endpoint = remote_endpoint_;
    std::cout << "Adding client to matchmaking" << std::endl;
    auto& client = get_client(client_endpoint);
    std::cout << "Adding client to matchmaking: " << client.get_ip() << ":" << client.get_port() << std::endl;
    matchmaking_system_.addPlayerToQueue(client);
}

/**
* @brief Sends the lobby information to the client
* @param client_endpoint The endpoint of the client to send the lobby information to
*/
void UdpServer::send_lobby_info(const std::string& message) {
  	const udp::endpoint& client_endpoint = remote_endpoint_;
  	std::cout << "Sending lobby info" << std::endl;
    std::cout << get_client(client_endpoint).get_lobby_id() << std::endl;
	auto lobby = lobbies_.at(get_client(client_endpoint).get_lobby_id());
        std::cout << "Sending lobby info to: " << client_endpoint << std::endl;
    std::string messages = "LOBBY_INFO|" + std::to_string(lobby.get_id()) + "|";
    for (const auto& cli : lobby.get_clients()) {
        messages += cli.get_nickname() + "/" + cli.get_ip() + ":" + cli.get_port() + "/";
        if (cli.is_ready()) {
            messages += "ready|";
        } else {
            messages += "not ready|";
        }	
    }
    std::cout << "Sending lobby info: " << messages << " to: " << client_endpoint << std::endl;
}

/**
* @brief handle the login of a client
* @param message The message containing the login information
*/
void UdpServer::handle_login(const std::string& message) {
    std::string username, password;
    std::istringstream iss(message.substr(6));

    std::getline(iss, username, ' ');
    std::getline(iss, password);

    if (!username.empty() && !password.empty() && password.find(' ') == std::string::npos) {
        handle_new_connection(remote_endpoint_, username, password);
    } else {
        std::cout << "Invalid login format" << std::endl;
        throw std::runtime_error("Invalid login format");
//        auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::ACK, socket_);
//        if (typeid(*packet) == typeid(PacketACK)) {
//            dynamic_cast<PacketACK*>(packet.get())->format_data(false);
//        }
//        packet->send_packet(remote_endpoint_);
    }
}

void UdpServer::get_best_score_cli(std::string message) {
    server::client& client = get_client(remote_endpoint_);
    uint64_t bs = client.get_best_score();
    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMD, socket_);
    if (typeid(*packet) == typeid(PacketCMD)) {
        dynamic_cast<PacketCMD*>(packet.get())->format_data(std::to_string(bs));
    }
    packet->send_packet(remote_endpoint_);
}

void UdpServer::new_score_cli(std::string message) {
    server::client& client = get_client(remote_endpoint_);
    try {
        const int lobby_id = std::stoi(message.substr(sizeof("new_score")));
        client.new_score(lobby_id);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
* @brief execute a function based on the message
* @param message The message containing the function to execute
* @return true if the function was executed, false otherwise
*/
void UdpServer::execute_function(const std::string& message, std::string client_str) {
  	auto it = std::find_if(connected_clients_.begin(), connected_clients_.end(), [&client_str](const server::client& cli) {
    	return cli.get_ip() + ":" + cli.get_port() == client_str;
    });
    if (message.rfind("login") == 0 || it != connected_clients_.end()) {
        std::cout << "Received: " << message << " from: " << client_str << std::endl;
        bool command_found = false;
        for (const auto& [command, func] : function_map_) {
            if (message.find(command) == 0) {
                try {
                    func(message);
                    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::ACK, socket_);
                    if (typeid(*packet) == typeid(PacketACK)) {
                        dynamic_cast<PacketACK*>(packet.get())->format_data(true);
                    }
                    packet->send_packet(remote_endpoint_);
                } catch (std::exception& e) {
                    std::cerr << e.what() << std::endl;
                    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::ACK, socket_);
                    if (typeid(*packet) == typeid(PacketACK)) {
                        dynamic_cast<PacketACK*>(packet.get())->format_data(false);
                    }
                    packet->send_packet(remote_endpoint_);
                }
                command_found = true;
                break;
            }
        }
        if (!command_found) {
            std::cout << "Unknown command: " << message << std::endl;
            auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::ACK, socket_);
            if (typeid(*packet) == typeid(PacketACK)) {
                dynamic_cast<PacketACK*>(packet.get())->format_data(false);
            }
            packet->send_packet(remote_endpoint_);
        } else {
            std::cout << "Not login client: " << client_str << std::endl;
        }
    }
}

/**
 * @brief Handles a client message by processing commands or handling client connection state.
 *
 * @param message The message sent by the client.
 * @param client_endpoint The client's endpoint.
 */
void UdpServer::handle_client_message(const std::string& msg, const asio::ip::udp::endpoint& client_endpoint, std::size_t bytes_recv) {

    std::string client_str = client_endpoint.address().to_string() + ":" + std::to_string(client_endpoint.port());
    std::cout << "sender: " << client_str << std::endl;
    std::cout << "Authorised clients:" << std::endl;
    for (const auto& client : connected_clients_) {
        std::cout << "\t" << client << std::endl;
    }
    std::string message = "null";
    try {
        uint32_t type = Packet::extract_type(msg.data(), bytes_recv);
        message = Packet::extract_data(msg.data(), bytes_recv, type);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    if (message == "score") { // TODO : FOR TEST ONLY, GONNA BE REMOVED
        server::ClientSaver cs("clients.csv");
        try {
            auto test = cs.get_all_best_scores();
            for (const auto& [username, score] : test) {
                std::cout << "Username: " << username << " Score: " << score << std::endl;
            }
       } catch (const server::ClientSaver::ClientSaverException& e) {
            std::cerr << e.what() << std::endl;
       }
    }
    if (message == "start") {
      	try {
            ping_to_choose_host(remote_endpoint_);
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } else {
      	try {
            execute_function(message, client_str);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
 	}
}

/**
 * @brief Handles the matchmaking queue by checking if there are enough players to start a game.
 */
void UdpServer::handle_matchmaking_queue() {
    std::cout << "Handling matchmaking queue" << std::endl;
    matchmaking_system_.matchPlayers();
    std::cout << "Client Matched" << std::endl;
    try {
        auto &lobby = matchmaking_system_.getLobby();
        if (!lobby.is_empty()) {
            std::cout << "Lobby ID: " << lobby.get_id() << std::endl;
            std::cout << "Host: " << lobby.get_host() << std::endl;
            std::cout << "Clients:" << std::endl;
            for (const auto& cli : lobby.get_clients()) {
                std::cout << "\t" << cli << std::endl;
            }
            lobbies_.insert(std::make_pair(lobby.get_id(), lobby));
            std::cout << "Lobby created" << std::endl;
            auto &client_host = get_client(lobby.get_host().get_endpoint());
            client_host.set_lobby_id(lobby.get_id());
            client_host.set_ready(false);
            ping_to_choose_host(lobby.get_host().get_endpoint());
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "Matchmaking queue handled" << std::endl;
}

/**
 * @brief Main server loop. Waits for messages and processes them.
 */
void UdpServer::server_loop() {
    while (true) {
        udp::endpoint client_endpoint;
        std::string message = "";
        std::size_t bytes_receive= 0;
        {
            std::unique_lock<std::mutex> lock(messages_mutex_);
            // Wait until there are messages to process
            messages_condition_.wait(lock, [this] { return !received_messages_.empty(); });

            // Process the message
            int message_id = received_messages_.begin()->first;
            std::pair<std::array<char, 65535>, udp::endpoint> pair_data = received_messages_.begin()->second;
            message = pair_data.first.data();
            client_endpoint = pair_data.second;
            bytes_receive = recv_buffer_size_;

            std::cout << "Received: " << message << " from " << client_endpoint << std::endl;
            received_messages_.erase(message_id);
        }
        // Call the function to handle the client message
        handle_client_message(message, client_endpoint, bytes_receive);
        handle_matchmaking_queue();
    }
}

void UdpServer::handle_new_connection(const udp::endpoint& client_endpoint, const std::string& username, const std::string& password) {
    std::string client_address = client_endpoint.address().to_string();
    std::string client_port = std::to_string(client_endpoint.port());
    server::ClientSaver cs("clients.csv");
    bool status = true;

    auto it = std::find_if(connected_clients_.begin(), connected_clients_.end(), [&client_endpoint](const server::client& cli) {
        return cli.get_id() == std::hash<std::string>{}(client_endpoint.address().to_string() + std::to_string(client_endpoint.port()));
    });

    try {
        uint32_t id = cs.check_if_user_already_exists_in_db(username, password);
        if (id) {
            try {
                auto existing_client = std::remove_if(connected_clients_.begin(), connected_clients_.end(), [&username](const server::client& cli) {
                    return cli.get_nickname() == username;
                });
                connected_clients_.erase(existing_client, connected_clients_.end());

                connected_clients_.emplace_back(client_address, client_port, username, password, id);
                connected_clients_.back().new_score(cs.get_best_score_by_id(id));
                std::cout << "New authorised client from db: " << client_address << ":" << client_port << std::endl;
            } catch (const server::client::ClientException& e) {
                std::cout << e.what() << std::endl;
            }
        } else if (it == connected_clients_.end()) {
            connected_clients_.emplace_back(client_address, client_port, username, password);
            connected_clients_.back().new_score(0);
            std::cout << "New authorised client: " << client_address << ":" << client_port << std::endl;
            cs.save_client(connected_clients_.back());
        } else {
            std::cout << "Client already authorised: " << client_address << ":" << client_port << std::endl;
        }
    } catch (const server::ClientSaver::ClientSaverException& e) {
        std::cerr << e.what() << std::endl;
        status = false;
    } catch (const server::client::ClientException& e) {
        std::cout << e.what() << std::endl;
        std::cout << "Password or Username is not correct" << std::endl;
        status = false;
    }
    if (status == false) {
        throw std::runtime_error("Error handling new connection");
    }
}

void UdpServer::send_message(const std::string &message,
                             const udp::endpoint &endpoint) {
    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMD, socket_);
    if (typeid(*packet) == typeid(PacketCMD)) {
        dynamic_cast<PacketCMD*>(packet.get())->format_data(message);
    }
    packet->send_packet(endpoint);
    std::cout << *packet << std::endl;

}

/**
* @brief Checks if all clients in a lobby are ready.
* @param lobby The lobby to check.
*/
bool UdpServer::everyone_ready(std::vector<server::client> clients) {
    for (const auto& cli : clients) {
        if (!cli.is_ready()) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Launches a game with the provided clients.
 *
 * @param clients The clients to launch the game with.
 */
void UdpServer::lauch_game(const std::vector<server::client>& clients) {
      auto cli_tmp = clients[0];

    for (auto& cli : clients) {
        if (cli_tmp != cli) {
          	std::cout << "Client latency: " << cli.get_latency() << std::endl;
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
* @brief set elapsed time
* @return the elapsed time
*/
std::chrono::milliseconds UdpServer::set_elapsed_time(std::vector<server::client>& clients) {
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
    return std::chrono::milliseconds(elapsed_time);
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
    try {
        auto &client = get_client(client_endpoint);
        if (client.is_ready()) {
            lobbies_.at(client.get_lobby_id()).mark_unready(client);
        } else {
            lobbies_.at(client.get_lobby_id()).mark_ready(client);
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    if (lobby_id == -42) {
        return;
    }

    std::vector<server::client> clients = lobbies_.at(lobby_id).get_clients();
    if (!everyone_ready(clients)) {
        std::cout << "Not everyone is ready" << std::endl;
        return;
    }
    set_elapsed_time(clients);
    lauch_game(clients);
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