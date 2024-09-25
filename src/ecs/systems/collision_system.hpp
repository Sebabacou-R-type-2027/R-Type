/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collision_system
*/

#pragma once

#include "registry.hpp"
#include "components.hpp"
#include <SFML/Graphics.hpp>

namespace ecs::systems {

    class CollisionSystem {
        public:
            void update(Registry& registry, sf::RenderWindow& window);
    };

}
