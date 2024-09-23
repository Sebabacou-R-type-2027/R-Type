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
#include "client.hpp"

class Lobby {
    public:
        Lobby(int id, client host) : id_(id), host_(host) {}
        ~Lobby() = default;
        void add_client(const client& cli);
        void remove_client(const client& cli);
        void mark_ready(const client& cli);
        void mark_unready(const client& cli);
        [[nodiscard]] bool is_everyone_ready() const;
        [[nodiscard]] const std::vector<client>& get_clients() const;
        void set_host(const client& cli);
        [[nodiscard]] int get_id() const;
        [[nodiscard]] client get_host() const;
        [[nodiscard]] bool is_host(const client& cli) const;
        [[nodiscard]] bool is_in_lobby(const client& cli) const;
        [[nodiscard]] bool is_full() const;
        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] bool is_lobby_id(int id) const { return id_ == id; }

    private:
        std::vector<client> clients_lobby_;
        client host_;
        bool everyone_ready_ = false;
        int id_;

};



#endif //LOBBY_HPP
