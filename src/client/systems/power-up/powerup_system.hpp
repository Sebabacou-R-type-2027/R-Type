/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#pragma once

#include "registry.hpp"
#include "../utils/Settings.hpp"

namespace ecs::systems {
    class PowerUp {
        public:
            void createCardPower(ecs::Registry& registry, sf::RenderWindow& window);
            bool threeShootPower(ecs::Registry& registry);
            void update();

        private:
            bool isCreate = false;
            sf::Font font;
            bool threeShootPower_activate = false;
            sf::Clock timer;
    };
}
