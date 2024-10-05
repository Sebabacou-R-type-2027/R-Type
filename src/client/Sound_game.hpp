#pragma once

#include "registry.hpp"
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

namespace rtype::game {
class Sound_game {
    public:
        Sound_game();
        ~Sound_game();
        void playSound(const std::string& filename);
        void playMusic(const std::string& filename);

    protected:
    private:
        std::vector<sf::Sound> sounds;
        sf::Sound test;
        sf::Music music;
};
}
