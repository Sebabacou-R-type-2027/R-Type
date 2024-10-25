/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Handle_collision
*/

#include "registry.hpp"
#include <networkClient/Client.hpp>

namespace ecs {
    /**
     * @brief Class that contains all the logic of the collision system
     */
    class HandleCollision {
        public:
            HandleCollision() = default;
            ~HandleCollision() = default;
            /**
             * @brief Update the collision system
             * 
             * @param registry contains all the logic of the components
             */
            void handle_collision(Registry& registry, client::Client& network, bool isSolo);

            /**
             * @brief Handle collision for network
             * 
             * @param i Index of the entity
             * @param registry contains all the logic of the components
             */
            void solo_handle_collision(std::size_t i, Registry& registry);

            /**
             * @brief Handle collision for solo
             * 
             * @param i Index of the entity
             * @param registry contains all the logic of the components
             * @param network contains all the logic of the network
             */
            void network_handle_collision(std::size_t i, Registry& registry, client::Client& network);
            /**
             * @brief Create explosion
             * 
             * @param registry contains all the logic of the components
             * @param x Position on the x axis
             * @param y Position on the y axis
             */
            void create_explosion(Registry& registry, float x, float y);
    };
}
