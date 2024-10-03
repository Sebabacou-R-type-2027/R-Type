/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Sound_game
*/

#pragma once

#include "registry.hpp"
#include <SFML/Audio.hpp>

class Sound_game {
    public:
        Sound_game();
        ~Sound_game();
        void playSound(const std::string& filename);
        void playMusic(const std::string& filename);
    protected:
    private:
        std::vector<sf::Sound> sounds;
        sf::Music music;
};
