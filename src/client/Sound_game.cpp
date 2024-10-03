/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Sound_game copy
*/

#include "Sound_game.hpp"
#include <iostream>
#include <SFML/Audio.hpp>

namespace rtype::game {

    void Sound_game::playSound(const std::string& filename) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(filename)) {
            sf::Sound sound;
            sound.setBuffer(buffer);
            sound.play();
            sounds.push_back(sound);
        }
    }

    void Sound_game::playMusic(const std::string& filename) {
        if (!music.openFromFile(filename)) {
            exit(1);
        }
        music.setLoop(true);
        music.play();
    }
}
