/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** button_system
*/

#pragma once

#include "registry.hpp"
#include "components/Button.hpp"
#include <SFML/System/Clock.hpp>

namespace ecs::systems {

    /**
     * @brief Class that contains all the logic of the button system
     */
    class ShaderSystem {
        public:
            /**
             * @brief Update the button system
             *
             * @param registry contains all the logic of the components
             * @param window contains the window of the game
             * @param shader contains the shader of the game
             */
            void update(Registry& registry, sf::RenderWindow& window, sf::Shader& shader);
            /**
             * @brief Render the button system
             *
             * @param registry contains all the logic of the components
             * @param window contains the window of the game
             * @param shader contains the shader of the game
             */
            void render(Registry& registry, sf::RenderWindow& window, sf::Shader& shader);
        private:
            sf::Clock clock; ///< Clock of the system
    };
}