#include "Sound_game.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

namespace rtype::game {



Sound_game::Sound_game()
{
}

Sound_game::~Sound_game()
{
}

void Sound_game::playSound(const std::string& filename)
{
    buffers.emplace_back();
    if (!buffers.back().loadFromFile(filename)) {
        std::cerr << "Erreur: Impossible de charger le fichier audio " << filename << std::endl;
        buffers.pop_back();
        return;
    }
    sounds.emplace_back();
    sounds.back().setBuffer(buffers.back());
    sounds.back().play();
}

void Sound_game::playMusic(const std::string& filename)
{
    if (!music.openFromFile(filename)) {
        std::cerr << "Erreur: Impossible de charger la musique " << filename << std::endl;
        return;
    }
    music.play();
}
}
