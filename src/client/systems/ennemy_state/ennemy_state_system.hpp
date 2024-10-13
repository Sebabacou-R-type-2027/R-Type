/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ennemy_state_system
*/

#include "registry.hpp"

namespace ecs::systems {

    /**
     * @brief Class that contains all the logic of the ennemy state system
     */
    class EnemyStateSystem {
        public:
            /**
             * @brief Update the ennemy state system
             *
             * @param registry contains all the logic of the components
             * @param deltaTime contains the time between two frames
             */
            void update(Registry& registry, float deltaTime);
    };
}
