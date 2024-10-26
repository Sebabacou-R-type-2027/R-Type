/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sound_effect
*/

#include "sound_effect.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

namespace ecs {

void SoundEffect::playSound(const std::string& filename, float volume = 50)
{
    buffers.emplace_back();
    if (!buffers.back().loadFromFile(filename)) {
        std::cerr << "Erreur: Impossible de charger le fichier audio " << filename << std::endl;
        buffers.pop_back();
        return;
    }
    sounds.emplace_back();
    sounds.back().setBuffer(buffers.back());
    sounds.back().setVolume(volume);
    sounds.back().play();
}

void SoundEffect::playMusic(const std::string& filename, float volume, bool loop)
{
    if (!music->openFromFile(filename)) {
        std::cerr << "Erreur: Impossible de charger la musique " << filename << std::endl;
        return;
    }
    music->setLoop(loop);
    music->setVolume(volume);
    music->play();
}
}
