
#include "client.hpp"
#include <chrono>

client::client(const std::string& ip, const std::string& port, const std::string& nickname) {
    if (ip.empty() || port.empty()) {
        throw ClientException("IP or port cannot be empty", *this);
    }
    set_nickname(nickname);
    this->ip_ = ip;
    this->port_ = port;
    this->id_ = generate_id();
    this->host_ = false;
    this->in_game_ = false;
}

std::string client::get_ip() const {
    return this->ip_;
}

std::string client::get_port() const {
    return this->port_;
}

std::string client::get_nickname() const {
    return this->nickname_;
}

void client::set_nickname(const std::string& nickname) {
    if (nickname.empty()) {
        throw ClientException("Nickname cannot be empty", *this);
    }
    this->nickname_ = nickname;
}

uint32_t client::get_id() const {
    return this->id_;
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

bool client::is_ready() const {
    return this->is_ready_;
}

void client::set_ready(const bool ready) {
    this->is_ready_ = ready;
}

void client::set_in_game(bool in_game) {
    this->in_game_ = in_game;
}

void client::start_game() {
    this->in_game_ = true;
}

uint32_t client::generate_id() const {
    auto timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return std::hash<std::string>{}(this->ip_ + this->port_ + std::to_string(timestamp));
}

std::ostream& operator<<(std::ostream& os, const client& cli) {
    os << "Client[ID: " << cli.id_
       << ", IP: " << cli.ip_
       << ", Port: " << cli.port_
       << ", Nickname: " << cli.nickname_
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

client& client::operator=(const client& cli) = default;