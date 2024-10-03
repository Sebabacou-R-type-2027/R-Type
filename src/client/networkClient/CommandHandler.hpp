//
// Created by shooting_star_t33 on 10/2/24.
//

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace client {
    class Client;
}

class CommandHandler {
     public:
            CommandHandler(client::Client& client) ;
            ~CommandHandler() = default;
            void addCommand(const std::string &command);
            void removeCommand(const std::string &command);
            void fillCommands();
            std::map<std::string, std::string> getCommands() const;
     private:
          std::string get_current_timestamp();
          std::map<std::string, std::string> _commands;
          client::Client& _client;
};



#endif //COMMANDHANDLER_HPP
