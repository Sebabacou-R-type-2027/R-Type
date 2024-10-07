/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Handle_collision
*/

#include "Handle_collision.hpp"

namespace ecs {

    void HandleCollision::handle_collision(Registry& registry) {
        auto& CollisionStates = registry.get_components<CollisionState>();
        auto& EntityTypes = registry.get_components<EntityType>();

        std::vector<std::size_t> entities_to_kill;

        for (std::size_t i = 0; i < CollisionStates.size(); ++i) {
            if (CollisionStates[i] && CollisionStates[i]->active) {
                for (std::size_t j = i + 1; j < CollisionStates.size(); ++j) {
                    if (CollisionStates[j] && CollisionStates[j]->active) {
                        if (i < EntityTypes.size() && j < EntityTypes.size()) {
                            if (EntityTypes[i]->current_type != EntityTypes[j]->current_type) {
                                entities_to_kill.push_back(i);
                                entities_to_kill.push_back(j);
                                std::cout << "Collision detected between entity " << i << " and entity " << j << std::endl;
                            }
                        }
                    }
                }
            }
        }

        try {
            for (auto entity_index : entities_to_kill)
                registry.kill_entity(registry.entity_from_index(entity_index));
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
