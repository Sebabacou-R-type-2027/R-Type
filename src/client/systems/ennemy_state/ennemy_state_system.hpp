/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ennemy_state_system
*/

#include "registry.hpp"

namespace ecs::systems {

    class EnemyStateSystem {
        public:
            void update(Registry& registry, float deltaTime);
    };
}
