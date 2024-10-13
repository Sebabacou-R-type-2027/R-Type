/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Settings
*/

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

class Settings {
public:
    bool isShaderEnabled = true;
    std::string serverAddress = "";
    int serverPort = 0;

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
