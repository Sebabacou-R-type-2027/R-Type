/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sound_effect
*/

#pragma once

#include "registry.hpp"
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <memory>

namespace ecs {

    /**
     * @brief Class that contains the sound of the game
     * 
     * @tparam Sound_game
     */
    class SoundEffect {
        public:
            SoundEffect() = default;
            ~SoundEffect() = default;
            /**
             * @brief Play a sound
             *
             * @param filename Path to the sound
             * @param volume Volume of the sound
             */
            void playSound(const std::string& filename, float volume);
            /**
             * @brief Play a music
             *
             * @param filename Path to the music
             * @param volume Volume of the music
             */
            void playMusic(const std::string& filename, float volume = 50,bool loop = false);


        private:
            std::vector<sf::Sound> sounds; ///< Vector of sounds
            std::vector<sf::SoundBuffer> buffers; ///< Vector of sound buffers
            std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>(); ///< Music of the game
    };

}
