//
// Created by shooting_star_t33 on 9/24/24.
//

#include "Client.hpp"

#include "PacketACK.hpp"
#include "PacketCMDP.hpp"

namespace client {
    Client::Client(asio::io_context& io_context, const std::string& server_ip, short server_port)
        :   io_context_(io_context),
            socket_(io_context, udp::endpoint(udp::v4(), 0)),
            remote_endpoint_(asio::ip::address::from_string(server_ip), server_port),
            is_running_(false) {

        receive_thread_ = std::thread(&Client::receive_loop, this);
        main_loop();
    }

    Client::~Client() {
        is_running_ = false;  // Arrête la boucle de réception
        if (receive_thread_.joinable()) {
            receive_thread_.join();  // Attends que le thread termine
            if (socket_.is_open()) {
                socket_.close();
            }
        }
    }

    void Client::send_message(const std::string &message) {
        if (message.empty()) {
            return ;
        }
        try {
            if (message == "ping") {
                auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::PING, socket_);
                packet->format_data();
                packet->send_packet(remote_endpoint_);
                std::cout << *packet << std::endl;
            } if (message == "ACK|0" || message == "ACK|1") {
                bool status = (message == "ACK|0") ? 0 : (message == "ACK|1") ? 1 : -1;
                auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::ACK, socket_);
                if (typeid(*packet) == typeid(PacketACK)) {
                    dynamic_cast<PacketACK*>(packet.get())->format_data(status);
                }
                packet->send_packet(remote_endpoint_);
                std::cout << *packet << std::endl;
            } if (message.find("CMDP|") == 0) {
                uint8_t cmd = 0;
                try {
                    std::string command = message.substr(message.find('|') + 1);
                    cmd = stoi(command);
                } catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                }
                auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMDP, socket_);
                if (typeid(*packet) == typeid(PacketCMDP)) {
                    dynamic_cast<PacketCMDP*>(packet.get())->format_data(cmd);
                }
                packet->send_packet(remote_endpoint_);
                std::cout << *packet << std::endl;
            }
            else {
                auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMD, socket_);
                if (typeid(*packet) == typeid(PacketCMD)) {
                    dynamic_cast<PacketCMD*>(packet.get())->format_data(message);
                }
                packet->send_packet(remote_endpoint_);
                std::cout << *packet << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void Client::send_message_to_player(const std::string& message, const udp::endpoint& player_endpoint) {
//        socket_.send_to(asio::buffer(message), player_endpoint);
          auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMD, socket_);
          if (typeid(*packet) == typeid(PacketCMD)) {
              dynamic_cast<PacketCMD*>(packet.get())->format_data(message);
          }
          packet->send_packet(player_endpoint);
          std::cout << *packet << std::endl;
    }

    void Client::main_loop() {
        std::string input;
        while (!is_running_) {
            std::cout << "Enter message to send (or type 'exit' to quit): ";
            std::getline(std::cin, input);

            if (input == "exit") {
                is_running_ = false;
                break;
            }

            send_message(input);
        }
    }

    void Client::send_packet(Packet& packet) const {
        packet.format_data();
        packet.send_packet(remote_endpoint_);
    }

    void Client::receive_loop() {
        start_receive();
        io_context_.run();
    }

    void Client::fillCommandsToSends(std::string command) {
        _commandsToSend[command] = command;
    }

    void Client::start_receive() {
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

    void Client::manage_message(std::size_t bytes_transferred) {
        std::string message = std::string(recv_buffer_.data(), bytes_transferred);
        uint32_t type = PacketFactory::TypePacket::PING;
        std::string data = "";
        try {
            type = Packet::extract_type(recv_buffer_.data(), bytes_transferred);
            data = Packet::extract_data(recv_buffer_.data(), bytes_transferred, type);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        if (!im_host) {
            if (type == PacketFactory::TypePacket::PING) {
              std::cout << "Received: PING" << std::endl;
              auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::PING, socket_);
              send_packet(*packet);
              return ;
            }
            if (type == PacketFactory::TypePacket::CMD) {
                std::cout << "Received: CMD" << std::endl;
                std::cout << data << std::endl;
                if (data.find("GAME_LAUNCH|HOST") == 0) {
                    std::cout << "Received: GAME_LAUNCH|HOST" << std::endl;
                    im_host = true;
                    command_handler_ = std::make_unique<CommandHandler>(*this);
                } else if (data.find("GAME_LAUNCH|") == 0) {
                    std::cout << "Received: GAME_LAUNCH" << std::endl;
                    std::cout << data << std::endl;
                    std::string host_data = data.substr(data.find('|') + 1);
                    std::string host_ip = host_data.substr(0, host_data.find(':'));
                    std::string host_port = host_data.substr(host_data.find(':') + 1, host_data.rfind(':') - host_data.find(':') - 1);
                    std::string host_id = host_data.substr(host_data.rfind(':') + 1);
                    std::cout << "Host IP: " << host_ip << std::endl;
                    std::cout << "Host Port: " << host_port << std::endl;
                    remote_endpoint_ = udp::endpoint(asio::ip::address::from_string(host_ip), std::stoi(host_port));
                    send_message("GAME_LAUNCH|ACK");
                }
                return;
            }
        } else {
            std::cout << "Received from Host: " << data << std::endl;
            if (data.find("GAME_LAUNCH|ACK") == 0) {
                players_endpoints_[std::to_string(number_of_players_)] = remote_endpoint_;
                number_of_players_ += 1;
                send_message_to_player("Start the game", remote_endpoint_);
                return ;

            }
            if (type == PacketFactory::TypePacket::CMDP) {
                std::cout << "Received: CMDP" << std::endl;
                std::cout << data << std::endl;
                command_handler_.get()->addCommand(data);
                return ;
            }
        }
        std::cout << std::endl;
    }

    void Client::handle_receive(std::size_t bytes_transferred) {
        std::cout << "Handle receive: " << bytes_transferred << std::endl;
        if (bytes_transferred > 0) {
            std::lock_guard<std::mutex> lock(messages_mutex_);
            manage_message(bytes_transferred);

            auto message = std::string(recv_buffer_.data(), bytes_transferred);

            received_messages_[message_id_counter_] = std::make_pair(message, remote_endpoint_);

            message_id_counter_ += 1;
//            std::cout << "Message ID: " << message_id_counter_ << std::endl;

            std::fill(recv_buffer_.begin() + bytes_transferred, recv_buffer_.end(), 0);
            messages_condition_.notify_one();
        }
        start_receive();
    }

    void Client::insertCommands(const std::map<std::string, std::string>& commands) {
        for (const auto& command : commands) {
            _commandsToDo[command.first] = command.second;
        }
    }
}
