/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Handle_collision
*/

#include "Handle_collision.hpp"

namespace ecs {

    void HandleCollision::handle_collision(Registry& registry) {
        auto CollisionStates = registry.get_components<CollisionState>();
        auto EntityTypes = registry.get_components<EntityType>();

        std::vector<Entity> entities_to_kill;

        for (std::size_t i = 0; i < CollisionStates.size(); ++i)
            if (CollisionStates[i] && CollisionStates[i]->get().active)
                for (std::size_t j = 0; j < CollisionStates.size(); ++j)
                    if (CollisionStates[j] && CollisionStates[j]->get().active
                        && i < EntityTypes.size() && j < EntityTypes.size()
                        && EntityTypes[i] && EntityTypes[j]
                        && EntityTypes[i]->get().current_type != EntityTypes[j]->get().current_type) {
                            auto entity1 = registry.entity_from_index(i), entity2 = registry.entity_from_index(j);
                            if (!(entity1 || entity2))
                                continue;
                            entities_to_kill.push_back(*entity1);
                            entities_to_kill.push_back(*entity2);
                        }
        for (auto entity : entities_to_kill)
            registry.kill_entity(entity);
    }
}
