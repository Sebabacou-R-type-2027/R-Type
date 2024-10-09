/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Handle_enemies_wave
*/

#include "Handle_enemies_wave.hpp"

namespace ecs {
    void HandleEnemiesWave::update(Registry& registry) {
        auto enemyStates = registry.get_components<EnemyState>();
        auto loopMovements = registry.get_components<LoopMovement>();
        auto positions = registry.get_components<Position>();
        auto entityTypes = registry.get_components<EntityType>();

        for (std::size_t i = 0; i < enemyStates.size(); ++i) {
            if (enemyStates[i] && loopMovements[i] && positions[i] && entityTypes[i]) {
                if (enemyStates[i]->get().current_state == State::Patrolling) {
                    if (positions[i]->get().x < loopMovements[i]->get().min_x) {
                        loopMovements[i]->get().speed = -loopMovements[i]->get().speed;
                        positions[i]->get().x = loopMovements[i]->get().min_x;
                    } else if (positions[i]->get().x > loopMovements[i]->get().max_x) {
                        loopMovements[i]->get().speed = -loopMovements[i]->get().speed;
                        positions[i]->get().x = loopMovements[i]->get().max_x;
                    }
                    positions[i]->get().x += loopMovements[i]->get().speed;
                }
            }
        }
    }
}