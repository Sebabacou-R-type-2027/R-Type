/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Settings
*/

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>

class Settings {
public:
    bool isShaderEnabled = true;
    std::string serverAddress = "";
    int serverPort = 0;
    std::string username = "";
    std::string password = "";
    std::string CreateLobbyID = "";
    std::string JoinLobbyID = "";

    static Settings& getInstance() {
        static Settings instance;
        return instance;
    }

private:
    Settings() {}
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
};

#endif // SETTINGS_HPP
