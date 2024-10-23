/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Handle_collision
*/

#include "Handle_collision.hpp"

namespace ecs {

    void HandleCollision::create_explosion(Registry& registry, float x, float y) {
        auto explosion = registry.spawn_entity();
        auto &animation = registry.emplace_component<Animation>(explosion, "assets/sprites/r-typesheet3.gif", 12, 1, 0.1f);
        registry.emplace_component<Position>(explosion, x, y);
        registry.emplace_component<LifeState>(explosion, true);
    }
    void HandleCollision::handle_collision(Registry& registry) {
        auto &collisionstates = registry.get_components<CollisionState>();
        auto &entitytypes = registry.get_components<EntityType>();
        auto &positions = registry.get_components<Position>();
        auto &lifestates = registry.get_components<LifeState>();

        for (std::size_t i = 0; i < collisionstates.size() && i < entitytypes.size() && i < positions.size() && i < lifestates.size(); ++i)
            if (collisionstates[i] && collisionstates[i]->active)
                for (std::size_t j = 0; j < collisionstates.size() && j < entitytypes.size() && j < positions.size() && j < lifestates.size(); ++j)
                    if (collisionstates[j] && collisionstates[j]->active
                        && i < entitytypes.size() && j < entitytypes.size()
                        && entitytypes[i]->current_type != entitytypes[j]->current_type
                        && lifestates[i]->isAlive && lifestates[j]->isAlive) {
                            // create_explosion(registry, positions[i]->x, positions[i]->y);
                            // lifestates[i]->isAlive = false;
                            // lifestates[j]->isAlive = false;
                            // positions[i]->x = -1000;
                            // positions[j]->x = -1000;
                            // positions[i]->y = -1000;
                            // positions[j]->y = -1000;
                        }
    }
}
