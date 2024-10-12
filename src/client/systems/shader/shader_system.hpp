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

    class ShaderSystem {
        public:
            void update(Registry& registry, sf::RenderWindow& window, sf::Shader& shader);
            void render(Registry& registry, sf::RenderWindow& window, sf::Shader& shader);
        private:
            sf::Clock clock;
    };
}