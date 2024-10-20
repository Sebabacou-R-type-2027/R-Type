#include "CommandHandler.hpp"
#include "Client.hpp"

CommandHandler::CommandHandler(client::Client& client) : _client(client) {}
std::string CommandHandler::get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

    std::string timestamp = ss.str();
    std::string unique_str = timestamp + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    std::hash<std::string> hasher;
    return std::to_string(hasher(unique_str));
}

void CommandHandler::addCommand(const std::string &command) {
        _commands[get_current_timestamp()] = command;
}

void CommandHandler::removeCommand(const std::string &command) {
    for (auto it = _commands.begin(); it != _commands.end(); ++it) {
        if (it->second == command) {
            _commands.erase(it);
            break;
        }
    }
}

void CommandHandler::fillCommands() {
    _client.insertCommands(_commands);
}

std::map<std::string, std::string> CommandHandler::getCommands() const {
    return _commands;
}