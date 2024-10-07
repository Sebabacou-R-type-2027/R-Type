#include <iostream>
#include "components/Entity_type.hpp"
#include "registry.hpp"

std::size_t CheckEntity(ecs::Registry& registry, ecs::Type type)
{
    std::size_t playerIndex = -1;
    auto& entityTypes = registry.get_components<ecs::EntityType>();

    for (std::size_t i = 0; i < entityTypes.size(); ++i) {
        if (entityTypes[i] && entityTypes[i]->current_type == type) {
            playerIndex = i;
            break;
        }
    }

    if (playerIndex == -1) {
        std::cerr << "Player entity not found!\n";
        return -1;
    }
    return playerIndex;
}