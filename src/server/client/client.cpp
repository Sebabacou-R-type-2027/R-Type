#include "client.hpp"
#include <chrono>

namespace server {
    client::client(const std::string& ip, const std::string& port, const std::string& username, const std::string& password) {
        if (ip.empty() || port.empty()) {
            throw ClientException("IP or port cannot be empty", *this);
        }
        set_nickname(username);
        this->ip_ = ip;
        this->port_ = port;
        this->id_ = generate_id();
        this->host_ = false;
        this->in_game_ = false;
        this->password_ = password; // TODO: hash the password
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
        if (username.empty()) {
            throw ClientException("username cannot be empty", *this);
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
        this->password_ = password;
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

    bool client::is_ready() const {
        return this->is_ready_;
    }

    void client::set_ready(const bool ready) {
        this->is_ready_ = ready;
    }

    void client::start_game() {
        this->in_game_ = true;
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

    bool operator==(const client& cli1, const client& cli2) {
        return cli1.get_id() == cli2.get_id();
    }

    bool operator!=(const client& cli1, const client& cli2) {
        return !(cli1 == cli2);
    }
}