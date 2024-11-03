#include "client.hpp"
#include "ClientSaver.hpp"
#include <chrono>
#include <iostream>

namespace server {
    client::client(const std::string& ip, const std::string& port, const std::string& username, const std::string& password, uint32_t id) {
        if (ip.empty() || port.empty()) {
            throw ClientException("IP or port cannot be empty", *this);
        }

        this->ip_ = ip;
        this->port_ = port;
        if (id == 0) {
            set_nickname(username);
            this->id_ = generate_id();
        } else {
            this->id_ = id;
            this->username_ = username;
        }
        this->host_ = false;
        this->in_game_ = false;
        this->password_ = hash_password(password);
    }

    std::string client::get_ip() const {
        return this->ip_;
    }

    std::string client::get_port() const {
        return this->port_;
    }

    std::string client::get_nickname() const {
        return this->username_;
    }

    void client::set_nickname(const std::string& username) {
        ClientSaver cs("clients.csv");
        if (username.empty()) {
            throw ClientException("username cannot be empty", *this);
        }

        try {
            if (cs.check_if_user_already_exists_in_db(username, "") != 0) {
                throw ClientException("Username already exists", *this);
            }
        } catch (const ClientException& e) {
            throw ClientException("Username already exists", *this);
        }
        this->username_ = username;
    }

    uint32_t client::get_id() const {
        return this->id_;
    }

    std::string client::get_password() const {
        return this->password_;
    }

    void client::set_password(const std::string& password) {
        this->password_ = hash_password(password);
    }

    asio::ip::udp::endpoint client::get_endpoint() const {
        return asio::ip::udp::endpoint(asio::ip::address::from_string(this->ip_), std::stoi(this->port_));
    }

    void client::set_endpoint(const asio::ip::udp::endpoint& endpoint) {
        this->ip_ = endpoint.address().to_string();
        this->port_ = std::to_string(endpoint.port());
        this->endpoint_ = endpoint;
    }

    bool client::is_host() const {
        return this->host_;
    }

    void client::set_host(bool host) {
        this->host_ = host;
    }

    bool client::is_in_game() const {
        return this->in_game_;
    }

    void client::set_in_game(bool in_game) {
        this->in_game_ = in_game;
    }

    uint32_t client::generate_id() const {
        auto timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        return std::hash<std::string>{}(this->ip_ + this->port_ + std::to_string(timestamp));
    }

    std::string client::hash_password(const std::string &password) {
        std::hash<std::string> hasher;
        return std::to_string(hasher(password));
    }

    bool client::is_ready() const {
        return this->is_ready_;
    }

    void client::set_ready(const bool ready) {
        this->is_ready_ = ready;
    }

    void client::start_game() {
        this->in_game_ = true;
        std::cout << "Client " << this->username_ << " is in game" << std::endl;
    }

    std::ostream& operator<<(std::ostream& os, const client& cli) {
        os << "Client[ID: " << cli.id_
           << ", IP: " << cli.ip_
           << ", Port: " << cli.port_
           << ", Username: " << cli.username_
           << ", Host: " << (cli.host_ ? "Yes" : "No")
           << ", In Game: " << (cli.in_game_ ? "Yes" : "No")
           << "]";
        return os;
    }

    uint32_t client::get_best_score() const {
        return this->best_score_;
    }

    bool client::new_score(uint32_t score) {
        if (score > this->best_score_) {
            server::ClientSaver cs("clients.csv");

            this->best_score_ = score;
            cs.change_field(this->id_, "BestScore", std::to_string(score));
            return true;
        }
        return false;
    }

    bool operator==(const client& cli1, const client& cli2) {
        return cli1.get_id() == cli2.get_id();
    }

    bool operator!=(const client& cli1, const client& cli2) {
        return !(cli1 == cli2);
    }

    void client::set_last_ping_time(std::chrono::steady_clock::time_point time) {
        last_ping_time_ = time;
    }

    std::chrono::steady_clock::time_point client::get_last_ping_time() const {
        return last_ping_time_;
    }

    void client::set_latency(long latency) {
        latency_ = latency;
    }

    long client::get_latency() const {
        return latency_;
    }

    void client::set_lobby_id(int lobby_id) {
        lobby_id_ = lobby_id;
    }

    int client::get_lobby_id() const {
        return lobby_id_;
    }
}