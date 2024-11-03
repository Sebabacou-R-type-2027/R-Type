//
// Created by shooting_star_t33 on 9/20/24.
//
#ifndef LOBBY_HPP
#define LOBBY_HPP

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <asio.hpp>
#include <unordered_map>
#include <iomanip>
#include "client/client.hpp"

class Lobby {
    public:
        Lobby(int id, server::client host) : id_(id), host_(host) {}
        ~Lobby() = default;
        void add_client(const server::client& cli);
        void remove_client(const server::client& cli);
        void mark_ready(const server::client& cli);
        void mark_unready(const server::client& cli);
        void set_client_ready(const server::client& cli, bool ready);
        [[nodiscard]] bool is_everyone_ready() const;
        [[nodiscard]] std::vector<server::client> get_clients() const;
        void set_host(const server::client& cli);
        [[nodiscard]] int get_id() const;
        [[nodiscard]] server::client get_host() const;
        [[nodiscard]] bool is_host(const server::client& cli) const;
        [[nodiscard]] bool is_in_lobby(const server::client& cli) const;
        [[nodiscard]] bool is_full() const;
        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] bool is_lobby_id(int id) const { return id_ == id; }
        void handle_chat_message(const std::string& message, const server::client& cli, std::size_t bytes_recv);
        std::vector<std::string>& get_chat_history();

    private:
        std::vector<server::client> clients_lobby_;
        std::vector<std::string> chat_history_;
        server::client host_;
        bool everyone_ready_ = false;
        int id_;
};



#endif //LOBBY_HPP
