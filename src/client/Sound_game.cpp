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
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "Erreur: Impossible de charger le fichier audio " << filename << std::endl;
        return;
    }
    printf("JULLLL");
    sounds.emplace_back();
    sounds.back().setBuffer(buffer);
    test.setBuffer(buffer);
    test.play();
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
