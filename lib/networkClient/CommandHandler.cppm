#if __cpp_lib_modules < 202207L
module;

#include <iostream>
#include <string>
#include <vector>
#endif

export module CommandHandler;

#if __cpp_lib_modules >= 202207L
import std;
#endif

import Client;

export class CommandHandler {
    public:
        CommandHandler(client::Client& client) : _client(client) {}
        ~CommandHandler() = default;

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

        void fillCommands() {
            _client.insertCommands(_commands);
        }

        std::map<std::string, std::string> CommandHandler::getCommands() const {
            return _commands;
        }
    private:
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

   private:
       std::map<std::string, std::string> _commands;
       client::Client& _client;
};