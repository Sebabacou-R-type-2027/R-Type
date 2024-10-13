/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** animation_system
*/

#pragma once

#include "registry.hpp"
#include <SFML/Graphics.hpp>

namespace ecs::systems {

    /**
     * @brief Class that contains all the logic of the animation system
     */
    class AnimationSystem {
        public:
            /**
             * @brief Update the animation system
             *
             * @param registry contains all the logic of the components
             * @param deltaTime contains the time between two frames
             * @param window contains the window of the game
             */
            void update(Registry& registry, float deltaTime, sf::RenderWindow& window);
        private:
    };

}