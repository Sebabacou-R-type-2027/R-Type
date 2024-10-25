/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Enemy_killed
*/

#include <vector>
#include <cstdint>

namespace ecs {
    struct KillLog {
        std::vector<std::uint32_t> killedEnemies;

        void addKill(std::uint32_t enemyID) {
            killedEnemies.push_back(enemyID);
        }
    };
}
