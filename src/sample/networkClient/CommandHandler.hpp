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
            /**
             * @brief Ajoute
             * 
             * @param command Command to add
             * @return void
             */
            void addCommand(const std::string &command);
            /**
             * @brief Remove a command from the command handler
             * 
             * @param command Command to remove
             * @return void
             */
            void removeCommand(const std::string &command);
            /**
             * @brief Get the command from the command handler
             * 
             * @param command Command to get
             * @return std::string Command
             */
            void fillCommands();
            /**
             * @brief Get the commands from the command handler
             * 
             * @return std::map<std::string, std::string> Commands
             */
            std::map<std::string, std::string> getCommands() const;
     private:
          std::string get_current_timestamp(); ///< Get the current timestamp
          std::map<std::string, std::string> _commands; ///< Commands
          client::Client& _client; ///< Client
};

#endif //COMMANDHANDLER_HPP
