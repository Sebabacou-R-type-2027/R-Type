module;
#if __cpp_lib_modules < 202207L

#include <iostream>
#include <memory>
#include <string>
#include <map>
#endif

#include "Packet.hpp"
#include "PacketACK.hpp"
#include "PacketCMD.hpp"
#include "PacketCMDP.hpp"
#include <asio.hpp>
#include <lz4.h>

export module Client;

#if __cpp_lib_modules >= 202207L
import std;
#endif

using namespace asio;
using namespace asio::ip;

std::string format_command(const std::string& command) {
    return (command.size() == 1) ? "0 " + command : command.substr(0, 1) + " " + command.substr(1, 1);
}

export class Client {
    public:
        Client(asio::io_context& io_context)
            : io_context_(io_context), socket_(io_context, udp::v4()), is_running_(false) {
        }

        ~Client() {
            is_running_ = false;
            if (receive_thread_.joinable()) {
                receive_thread_.join();
                if (socket_.is_open()) {
                    socket_.close();
                }
            }
            if (send_commands_thread_.joinable()) {
                send_commands_thread_.join();
            }
        }

        void main_loop() {
                std::string input;

                while (connected_ == false) {}
                while (!is_running_) {
                    std::cout << "$> ";
                    std::getline(std::cin, input);

                    if (input == "exit") {
                        is_running_ = false;
                        break;
                    }
                    if (input.rfind("chat:") == 0) {
                        std::string cmd_snd = "chat:" + std::to_string(input.size() - 4) + ":" + compressString(input.substr(5));
                        send_message(cmd_snd);
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        send_message("GET_CHAT");
                        std::cout << "INPUTTTTTT: " << cmd_snd << std::endl;
                        continue;
                    }
                    send_message(input);
                }
            }

        void connect(const std::string& server_ip, short server_port) {
            if (connected_ == true) {
                return;
            }
            socket_ = udp::socket(io_context_, udp::endpoint(udp::v4(), 0));
            remote_endpoint_ = udp::endpoint(asio::ip::address::from_string(server_ip), server_port);
            receive_thread_ = std::thread(&Client::receive_loop, this);
            send_commands_thread_ = std::thread(&Client::send_commands_to_all_clients, this);
            connected_ = true;
        }

        bool is_connected() const {
            return connected_;
        }

        void send_packet(Packet& packet) const {
            packet.format_data();
            packet.send_packet(remote_endpoint_);
        }


        void send_message(const std::string &message) {
            if (message.empty()) {
                return;
            }
            try {
                if (message == "ping") {
                    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::PING, socket_);
                    packet->format_data();
                    packet->send_packet(remote_endpoint_);
                } if (message == "ACK|0" || message == "ACK|1") {
                    bool status = (message == "ACK|0") ? 0 : (message == "ACK|1") ? 1 : -1;
                    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::ACK, socket_);
                    if (typeid(*packet) == typeid(PacketACK)) {
                        dynamic_cast<PacketACK*>(packet.get())->format_data(status);
                    }
                    packet->send_packet(remote_endpoint_);
                } if (message.find("CMDP|") == 0) {
              	    if (im_host) {
                        try {
                    	    std::string command = message.substr(message.find('|') + 1);
                    	    int temp_cmd = std::stoi(command);
                    	    if (temp_cmd < 0 || temp_cmd > 255) {
                        	    throw std::out_of_range("Command value out of range for uint8_t");
                    	    }
                		    command = std::to_string(my_id_in_lobby_) + command;
                		    this->addCommand(command);
                        } catch (const std::exception& e) {
                    	    std::cerr << "Error parsing command: " << e.what() << std::endl;
                        }
                        return;
                    }
                    uint8_t cmd = 0;
                    try {
                        std::string command = message.substr(message.find('|') + 1);
                        int temp_cmd = std::stoi(command);
                        if (temp_cmd < 0 || temp_cmd > 255) {
                            throw std::out_of_range("Command value out of range for uint8_t");
                        }
                        cmd = static_cast<uint8_t>(temp_cmd);
                    } catch (const std::exception& e) {
                        std::cerr << "Error parsing command: " << e.what() << std::endl;
                    }
                    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMDP, socket_);
                    if (typeid(*packet) == typeid(PacketCMDP)) {
                        dynamic_cast<PacketCMDP*>(packet.get())->format_data(cmd);
                    }
                    packet->send_packet(remote_endpoint_);
                } else {
                    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMD, socket_);
                    if (typeid(*packet) == typeid(PacketCMD)) {
                        dynamic_cast<PacketCMD*>(packet.get())->format_data(message);
                    }
                    std::cout << "Sending chat message: " << message << std::endl;
                    packet->send_packet(remote_endpoint_);
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }

        void handle_receive(std::size_t bytes_transferred, const udp::endpoint& remote_endpoint) {
            if (bytes_transferred > 0) {
                std::lock_guard<std::mutex> lock(messages_mutex_);
                manage_message(bytes_transferred, remote_endpoint);

                auto message = std::string(recv_buffer_.data(), bytes_transferred);

                received_messages_[message_id_counter_] = std::make_pair(message, remote_endpoint_);

                message_id_counter_ += 1;

                std::fill(recv_buffer_.begin() + bytes_transferred, recv_buffer_.end(), 0);
                messages_condition_.notify_one();
            }
            start_receive();
        }

        void insertCommands(const std::map<std::string, std::string>& commands) {
            for (const auto& command : commands) {
                _commandsToDo[command.first] = command.second;
            }
        }

        void fillCommandsToSends(std::string command) {
            _commandsSend[command] = command;
        }

        void send_command_with_ack(const std::string& command, const udp::endpoint& client_endpoint) {
            bool ack_received = false;
            int attempts = 0;
            constexpr int max_attempts = 5;

            while (!ack_received && attempts < max_attempts) {
                try {
                    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMDP, socket_);
                    if (typeid(*packet) == typeid(PacketCMDP)) {
                        dynamic_cast<PacketCMDP*>(packet.get())->format_data(std::stoi(command));
                    }
                    packet->send_packet(client_endpoint);
                    std::string formatted_data = format_command(command);
                    _commandsToDo[formatted_data.substr(0, formatted_data.find(' '))] = formatted_data.substr(formatted_data.find(' ') + 1);

                    ack_received = wait_for_ack(client_endpoint);
                    if (!ack_received) {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        ++attempts;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error in sending command with ack: " << e.what() << std::endl;
                }
            }

            if (!ack_received) {
                std::cerr << "Failed to send CMDP after " << max_attempts << " attempts." << std::endl;
            }
        }

        bool wait_for_ack(const udp::endpoint& client_endpoint) {
            std::unique_lock<std::mutex> lock(messages_mutex_);
            for (const auto& [id, message] : received_messages_) {
                if (message.second == client_endpoint) {
                    try {
                        uint32_t type = Packet::extract_type(message.first.data(), message.first.size());
                        if (type == PacketFactory::TypePacket::ACK) {
                            std::string data = Packet::extract_data(message.first.data(), message.first.size(), type);
                            int idp = Packet::extract_id(message.first.data(), message.first.size());
                            if (data == "1") {
                                received_messages_.erase(id);
                                return true;
                            } else {
                                received_messages_.erase(id);
                                return false;
                            }
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Error extracting packet type: " << e.what() << std::endl;
                    }
                }
            }
            return false;
        }

        std::string compressString(const std::string& data) {
            int maxCompressedSize = LZ4_compressBound(data.size());
            std::vector<char> compressedData(maxCompressedSize);

            int compressedSize = LZ4_compress_default(data.data(), compressedData.data(), data.size(), maxCompressedSize);
            if (compressedSize <= 0) {
                throw std::runtime_error("Erreur de compression LZ4");
            }

            compressedData.resize(compressedSize);
            return std::string(compressedData.begin(), compressedData.end());
        }

        std::string decompressString(const std::string& compressedData, size_t originalSize) {
            std::string decompressedData(originalSize, '\0');

            int decompressedSize = LZ4_decompress_safe(compressedData.data(), &decompressedData[0], compressedData.size(), originalSize);
            if (decompressedSize < 0) {
                throw std::runtime_error("Erreur de décompression LZ4");
            }

            return decompressedData;
        }

        std::vector<std::string> decouperMessages(const std::string& texte) {
            std::vector<std::string> chat_tmp;
            size_t pos = 0;
            std::string motif = "[";

            while (pos < texte.size()) {
                size_t debutMessage = texte.find(motif, pos);
                if (debutMessage == std::string::npos) {
                    break;
                }
                size_t debutProchainMessage = texte.find(motif, debutMessage + 1);
                if (debutProchainMessage != std::string::npos) {
                    chat_tmp.push_back(texte.substr(debutMessage, debutProchainMessage - debutMessage));
                    pos = debutProchainMessage;
                } else {
                    chat_tmp.push_back(texte.substr(debutMessage));
                    break;
                }
            }

            chat_.clear();
            for (auto& message : chat_tmp) {
                std::string removed_part = message.substr(0, 11);
                message = message.substr(11);
                removed_part += " " + message.substr(0, message.find(':')) + ":";
                message = message.substr(message.find(':') + 1);
                size_t message_size = std::stoi(message.substr(0, message.find(':')));
                message = message.substr(message.find(':') + 1);
                message = decompressString(message, message_size);
                std::cout << "Decompressed message: " << message << std::endl;
                chat_.push_back(removed_part + message);
            }

            return chat_;
        }

    private:
        void receive_loop() {
            start_receive();
            io_context_.run();
        }

        void start_receive() {
            socket_.async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            [this](std::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    handle_receive(bytes_recvd, remote_endpoint_);
                } else {
                    start_receive();
                }
            });
        }

        void manage_message(std::size_t bytes_transferred, const udp::endpoint& remote_endpoint) {
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
                return manage_player_msg(type, data);
            } else {
                return manage_host_msg(type, data, message, remote_endpoint);
            }
            std::cout << std::endl;
        }

        void manage_player_msg(uint32_t type, std::string data) {
            std::regex regex_pattern(R"(\[\d{2}:\d{2}:\d{2}\])");

            if (type == PacketFactory::TypePacket::PING) {
                std::cout << "Received: PING" << std::endl;
                auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::PING, socket_);
                send_packet(*packet);
                return;
            }
            if (type == PacketFactory::TypePacket::ACK) {
                std::cout << "Received: ACK[" << data << "]" << std::endl;
                return;
            }
            if (type == PacketFactory::TypePacket::CMDP) {
                std::string formatted_data = (data.size() == 1) ? "0 " + data : data.substr(0, 1) + " " + data.substr(1, 1);
                _commandsToDo[formatted_data.substr(0, formatted_data.find(' '))] = formatted_data.substr(formatted_data.find(' ') + 1);
                auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::ACK, socket_);
                if (typeid(*packet) == typeid(PacketACK)) {
                    dynamic_cast<PacketACK*>(packet.get())->format_data(true);
                }
                packet->send_packet(remote_endpoint_);
                return;
            }
            if (type == PacketFactory::TypePacket::CMD) {
                std::cout << "CMD received -> " << data << std::endl;
                if (data.find("START|") == 0) {
                    std::string host_data = data.substr(data.find('|') + 1);
                    my_id_in_lobby_ = std::stoi(host_data);
                    number_of_players_ += 1;
                }
                if (data.find("PJ|") == 0) {
                    std::string player_join = data.substr(data.find('|') + 1);
                    number_of_players_ += 1;
                }
                if (data.find("GAME_LAUNCH|HOST") == 0) {
                    std::cout << "You are the HOST" << std::endl;
                    im_host = true;
                } else if (data.find("GAME_LAUNCH|") == 0) {
                    std::string host_data = data.substr(data.find('|') + 1);
                    std::string host_ip = host_data.substr(0, host_data.find(':'));
                    std::string host_port = host_data.substr(host_data.find(':') + 1, host_data.rfind(':') - host_data.find(':') - 1);
                    std::string host_id = host_data.substr(host_data.rfind(':') + 1);
                    std::cout << "Host IP: " << host_ip << std::endl;
                    std::cout << "Host Port: " << host_port << std::endl;
                    remote_endpoint_ = udp::endpoint(asio::ip::address::from_string(host_ip), std::stoi(host_port));
                    send_message("GAME_LAUNCH|ACK");
                } else if (data[0] == '[') {
                    decouperMessages(data);

                    std::cout << "Chat messages: " << std::endl;
                    for (const auto& message : chat_) {
                        std::cout << message << std::endl;
                    }
                }
                return;
            }
        }

        void manage_host_msg(uint32_t type, std::string data, std::string message, const udp::endpoint& remote_endpoint) {
            std::string player_key = "Undefined";
            for (const auto& player : players_endpoints_) {
                if (player.second == remote_endpoint) {
                    player_key = std::to_string((std::stoi(player.first) + 1));
                    break;
                }
            }
            if (data.find("GAME_LAUNCH|ACK") == 0) {
                players_endpoints_[std::to_string(number_of_players_)] = remote_endpoint;
                my_id_in_lobby_ = number_of_players_;
                number_of_players_ += 1;
                message = "START|" + std::to_string(number_of_players_);
                send_message_to_player(message, remote_endpoint);

                std::string join_message = "PJ|" + std::to_string(number_of_players_);
                for (const auto& [_, endpoint] : players_endpoints_) {
                    if (endpoint != remote_endpoint) {
                        send_message_to_player(join_message, endpoint);
                    }
                }

                for (int i = 1; i < number_of_players_; ++i) {
                    std::string existing_player_message = "PJ|" + std::to_string(i);
                    send_message_to_player(existing_player_message, remote_endpoint);
                }
                return;
            }
            if (type == PacketFactory::TypePacket::ACK) {
                std::cout << "Result Packet = " << data << std::endl;
                return;
            }
            if (type == PacketFactory::TypePacket::CMDP) {
                std::cout << "Received CMDP : " << data << std::endl;
                data = player_key + data;
                this->addCommand(data);
                auto cmd = this->getCommands();
                return;
            }
        }

        void send_message_to_player(const std::string& message, const udp::endpoint& player_endpoint) {
            auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMD, socket_);
            if (typeid(*packet) == typeid(PacketCMD)) {
                dynamic_cast<PacketCMD*>(packet.get())->format_data(message);
            }
            packet->send_packet(player_endpoint);
        }

        void send_commands_to_all_clients() {
            while (!is_running_) {
                if (im_host) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    auto commands = this->getCommands();
                    for (const auto& [id, command] : commands) {
                        if (_commandsSend.find(id) == _commandsSend.end()) {
                            if (players_endpoints_.empty()) {
                                std::string formatted_data = (command.size() == 1) ? "0 " + command : command.substr(0, 1) + " " + command.substr(1, 1);
                                _commandsToDo[formatted_data.substr(0, formatted_data.find(' '))] = formatted_data.substr(formatted_data.find(' ') + 1);
                            } else {
                                for (const auto& endpoint : players_endpoints_) {
                                    send_command_with_ack(command, endpoint.second);
                                }
                            }
                        }
                        _commandsSend[id] = command;
                    }
                }
            }
        }

        void addCommand(const std::string &command) {
            _commands[get_current_timestamp()] = command;
        }

        void removeCommand(const std::string &command) {
            for (auto it = _commands.begin(); it != _commands.end(); ++it) {
                if (it->second == command) {
                    _commands.erase(it);
                    break;
                }
            }
        }

        std::map<std::string, std::string> getCommands() const {
            return _commands;
        }

        std::string get_current_timestamp() {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

            std::string timestamp = ss.str();
            std::string unique_str = timestamp + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
            std::hash<std::string> hasher;
            return std::to_string(hasher(unique_str));
        }

    public:
        int number_of_players_ = 0;
        int my_id_in_lobby_;
        std::map<std::string, std::string> _commandsToDo;
        std::atomic<bool> is_running_;
        std::vector<std::string> chat_ = {};
    private:
        bool im_host = false;
        asio::io_context& io_context_;
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::thread receive_thread_;
        std::thread send_thread_;
        std::unordered_map<std::string, udp::endpoint> players_endpoints_;
        std::mutex messages_mutex_;
        std::unordered_map<int, std::pair<std::string, udp::endpoint>> received_messages_;
        int message_id_counter_ = 0;
        std::condition_variable messages_condition_;
        std::array<char, 1024> recv_buffer_;
        std::size_t recv_buffer_size_ = 0;
        std::map<std::string, std::string> _commandsSend;
        std::thread send_commands_thread_;
        bool connected_ = false;
        std::map<std::string, std::string> _commands;

};
