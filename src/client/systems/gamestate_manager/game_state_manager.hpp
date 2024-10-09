/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_state_manager
*/

#include "registry.hpp"

namespace ecs::systems {

    class GameStateManager {
        public:
            void update(Registry& registry, float deltaTime);
    };
}
