/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** chasing_enemy_system
*/

#include "chasing_enemy_system.hpp"
#include <limits>
#include <math.h>

namespace ecs::systems {

    void ChasingEnemySystem::update(Registry& registry, sf::RenderWindow& window) {
        auto &positions = registry.get_components<Position>();
        auto &chasing = registry.get_components<Chasing>();
        auto playerEntities = registry.get_all_player_entity();

        for (std::size_t i = 0; i < positions.size() && i < chasing.size(); ++i) {
            if (positions[i] && chasing[i]) {
                Position &enemyPos = *positions[i];
                Chasing &chase = *chasing[i];

                float closestDistance = std::numeric_limits<float>::max();
                Position *closestPlayerPos = nullptr;

                for (auto playerEntity : playerEntities) {
                    auto &playerPos = positions[static_cast<std::size_t>(playerEntity)];
                    if (playerPos) {
                        float distance = sqrt(pow(playerPos->x - enemyPos.x, 2) + pow(playerPos->y - enemyPos.y, 2));

                        if (distance < closestDistance) {
                            closestDistance = distance;
                            closestPlayerPos = &*playerPos;
                        }
                    }
                }

                if (closestPlayerPos) {
                    if (closestPlayerPos->x > enemyPos.x) {
                        enemyPos.x += chase.speed;
                    } else {
                        enemyPos.x -= chase.speed;
                    }
                    if (closestPlayerPos->y > enemyPos.y) {
                        enemyPos.y += chase.speed;
                    } else {
                        enemyPos.y -= chase.speed;
                    }
                }
            }
        }
    }
}