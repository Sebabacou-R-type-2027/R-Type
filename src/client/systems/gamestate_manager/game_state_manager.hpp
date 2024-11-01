/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_state_manager
*/

#include "registry.hpp"

namespace ecs::systems {

    /**
     * @brief Class that contains all the logic of the game state manager
     */
    class GameStateManager {
        public:
            /**
             * @brief Update the game state manager
             *
             * @param registry contains all the logic of the components
             * @param deltaTime contains the time between two frames
             */
            void update(Registry& registry, float deltaTime);
    };
}
