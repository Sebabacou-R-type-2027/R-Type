/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** spawner_enemy_system
*/

#include "spawner_enemy_system.hpp"
#include <limits>
#include <math.h>

namespace ecs::systems {

    void SpawnerEnemySystem::update(Registry& registry, sf::RenderWindow& window) {
        auto &positions = registry.get_components<Position>();
        auto &spawner = registry.get_components<Spawner>();
        auto &Animations = registry.get_components<Animation>();

        for (std::size_t i = 0; i < positions.size() && i < spawner.size(); ++i) {
            if (positions[i] && spawner[i]) {
                Position &enemyPos = *positions[i];
                Spawner &spawn = *spawner[i];
                Animation &animation = *Animations[i];
                if (spawn.nbSpawned <= 0) {
                    break;
                }
                if (spawn.lastSpawn + spawn.cooldown < 0) {
                    _createEnemies.create_chasing_enemy(registry,
                        positions[i]->x, positions[i]->y,
                        1.0f,
                        "assets/Chasing_enemy/r-typesheet11_left.gif",
                        0.0f, window.getSize().x, 0.0f, window.getSize().y, 0.0f, 0.0f, 0.0f);
                    spawn.lastSpawn = 0;
                    spawn.nbSpawned -= 1;
                } else {
                    spawn.lastSpawn -= 1;
                }
            }
        }
    }
}