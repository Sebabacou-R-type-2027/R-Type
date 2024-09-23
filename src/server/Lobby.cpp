//
// Created by shooting_star_t33 on 9/20/24.
//

#include "Lobby.hpp"

void Lobby::add_client(const client& cli) {
    this->clients_lobby_.push_back(cli);
}

void Lobby::remove_client(const client& cli) {
    const auto it = std::find_if(clients_lobby_.begin(), clients_lobby_.end(), [&cli](const client& client) {
        return client.get_id() == cli.get_id();
    });

    if (it != clients_lobby_.end()) {
        clients_lobby_.erase(it);
    }
}

void Lobby::mark_ready(const client& cli) {
    const auto it = std::find_if(clients_lobby_.begin(), clients_lobby_.end(), [&cli](const client& client) {
        return client.get_id() == cli.get_id();
    });

    if (it != clients_lobby_.end()) {
        it->set_ready(true);
    }
}

void Lobby::mark_unready(const client& cli) {
    const auto it = std::find_if(clients_lobby_.begin(), clients_lobby_.end(), [&cli](const client& client) {
        return client.get_id() == cli.get_id();
    });

    if (it != clients_lobby_.end()) {
        it->set_ready(false);
    }
}

bool Lobby::is_everyone_ready() const {
    return std::all_of(clients_lobby_.begin(), clients_lobby_.end(), [](const client& cli) {
        return cli.is_ready();
    });
}

const std::vector<client>& Lobby::get_clients() const {
    return clients_lobby_;
}

bool Lobby::is_host(const client& cli) const {
    return host_ == cli;
}

bool Lobby::is_in_lobby(const client& cli) const {
    return std::any_of(clients_lobby_.begin(), clients_lobby_.end(), [&cli](const client& client) {
        return client.get_id() == cli.get_id();
    });
}

bool Lobby::is_full() const {
    return clients_lobby_.size() == 4;
}

bool Lobby::is_empty() const {
    return clients_lobby_.empty();
}

void Lobby::set_host(const client& cli) {
    host_ = cli;
}

client Lobby::get_host() const {
    return host_;
}

int Lobby::get_id() const {
    return id_;
}