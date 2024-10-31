#include "Client.hpp"

#include "PacketACK.hpp"
#include "PacketCMDP.hpp"

std::string format_command(const std::string& command) {
    return (command.size() == 1) ? "0 " + command : command.substr(0, 1) + " " + command.substr(1, 1);
}

namespace client {
    Client::Client(asio::io_context& io_context)
        : io_context_(io_context), socket_(io_context, udp::v4()), is_running_(false) {
    }

    Client::~Client() {
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

    void Client::send_commands_to_all_clients() {
        while (!is_running_) {
            if (im_host) {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                auto commands = command_handler_->getCommands();
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

    void Client::send_command_with_ack(const std::string& command, const udp::endpoint& client_endpoint) {
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

    bool Client::wait_for_ack(const udp::endpoint& client_endpoint) { // TODO : use a struct for received_messages_, id, size, timestamp, data, endpoint
	    std::unique_lock<std::mutex> lock(messages_mutex_);
        for (const auto& [id, message] : received_messages_) {
            if (message.second == client_endpoint) {
                try {
                    uint32_t type = Packet::extract_type(message.first.data(), message.first.size()); // TODO : check if timstamp is not too old
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

    void Client::send_message(const std::string &message) {
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
                		this->command_handler_->addCommand(command);
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
                packet->send_packet(remote_endpoint_);
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void Client::send_message_to_player(const std::string& message, const udp::endpoint& player_endpoint) {
          auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMD, socket_);
          if (typeid(*packet) == typeid(PacketCMD)) {
              dynamic_cast<PacketCMD*>(packet.get())->format_data(message);
          }
          packet->send_packet(player_endpoint);
    }

    void Client::connect(const std::string& server_ip, short server_port) {
        if (connected_ == true) {
            return;
        }
        socket_ = udp::socket(io_context_, udp::endpoint(udp::v4(), 0));
        remote_endpoint_ = udp::endpoint(asio::ip::address::from_string(server_ip), server_port);
        receive_thread_ = std::thread(&Client::receive_loop, this);
        send_commands_thread_ = std::thread(&Client::send_commands_to_all_clients, this);
        connected_ = true;
    }

    void Client::main_loop() {
        std::string input;

        while (connected_ == false) {}
        while (!is_running_) {
            std::cout << "$> ";
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
        _commandsSend[command] = command;
    }

    void Client::start_receive() {
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

    void Client::manage_player_msg(uint32_t type, std::string data) {
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
                command_handler_ = std::make_unique<CommandHandler>(*this);
            } else if (data.find("GAME_LAUNCH|") == 0) {
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
    }

    void Client::manage_host_msg(uint32_t type, std::string data, std::string message, const udp::endpoint& remote_endpoint) {
        std::string player_key = "Undefined";
        for (const auto& player : players_endpoints_) {
    		if (player.second == remote_endpoint) {
            	player_key = std::to_string((std::stoi(player.first) + 1));
        		break;
    		}
		}
//			std::cout << "Received from Player[" << player_key << "]: " << data << std::endl;
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
            this->command_handler_->addCommand(data);
            auto cmd = command_handler_->getCommands();
            return;
        }
    }

    void Client::manage_message(std::size_t bytes_transferred, const udp::endpoint& remote_endpoint) {
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

    void Client::handle_receive(std::size_t bytes_transferred, const udp::endpoint& remote_endpoint) {
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

    void Client::insertCommands(const std::map<std::string, std::string>& commands) {
        for (const auto& command : commands) {
            _commandsToDo[command.first] = command.second;
        }
    }
}