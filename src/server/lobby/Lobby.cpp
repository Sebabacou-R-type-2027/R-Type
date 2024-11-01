//
// Created by shooting_star_t33 on 9/20/24.
//

#include "Lobby.hpp"

void Lobby::add_client(const server::client& cli) {
    this->clients_lobby_.push_back(cli);
}

void Lobby::remove_client(const server::client& cli) {
    const auto it = std::find_if(clients_lobby_.begin(), clients_lobby_.end(), [&cli](const server::client& client) {
        return client.get_id() == cli.get_id();
    });

    if (it != clients_lobby_.end()) {
        clients_lobby_.erase(it);
    }
}

void Lobby::mark_ready(const server::client& cli) {
    for (auto& client : clients_lobby_) {
        if (client.get_id() == cli.get_id()) {
            client.set_ready(true);
            return;
        }
    }
}

void Lobby::mark_unready(const server::client& cli) {
    for (auto& client : clients_lobby_) {
        if (client.get_id() == cli.get_id()) {
            client.set_ready(false);
            return;
        }
    }
}

void Lobby::set_client_ready(const server::client& cli, bool ready) {
  	std::cout << "Setting client ready" << std::endl;
    for (auto& client : clients_lobby_) {
        if (client.get_id() == cli.get_id()) {
            client.set_ready(ready);
            return;
        }
    }
}

bool Lobby::is_everyone_ready() const {
  return std::all_of(clients_lobby_.begin(), clients_lobby_.end(),
                     [](const server::client &cli) { return cli.is_ready(); });
}

std::vector<server::client> Lobby::get_clients() const {
    return clients_lobby_;
}

bool Lobby::is_host(const server::client& cli) const {
    return host_ == cli;
}

bool Lobby::is_in_lobby(const server::client& cli) const {
    return std::any_of(clients_lobby_.begin(), clients_lobby_.end(), [&cli](const server::client& client) {
        return client.get_id() == cli.get_id();
    });
}

bool Lobby::is_full() const {
    return clients_lobby_.size() == 4;
}

bool Lobby::is_empty() const {
    return clients_lobby_.empty();
}

void Lobby::set_host(const server::client& cli) {
    host_ = cli;
}

server::client Lobby::get_host() const {
    return host_;
}

int Lobby::get_id() const {
    return id_;
}

/**
* @brief Handle chat message
* @param message The message containing the chat message
*/
void Lobby::handle_chat_message(const std::string& message, const server::client& cli, std::size_t bytes_recv) {
    std::cout << "Chat message: " << message << std::endl;

    std::string chat_message;

    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&current_time), "%H:%M:%S");
    std::string time = ss.str();

    chat_message = "[" + time + "] " + cli.get_nickname() + ":";
    for (std::size_t i = 0; i < bytes_recv; ++i) {
        chat_message += message[i];
    }
    chat_history_.push_back(chat_message);

    if (chat_history_.size() > 10) {
        chat_history_.erase(chat_history_.begin());
    }

    std::cout << "Chat history of lobby id = " << id_ << std::endl;
    for (auto& client : chat_history_) {
        std::cout << "		chat:" << client << "\n" << std::endl;
    }
}

std::vector<std::string>& Lobby::get_chat_history() {
    return chat_history_;
}