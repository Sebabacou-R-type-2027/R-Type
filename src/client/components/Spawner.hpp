/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Spawner
*/

#pragma once

namespace ecs {
    /**
     * @brief Class that contains the Spawner of an entity
     */
    struct Spawner {
        float cooldown;
        float lastSpawn;
        float nbSpawned;

        /**
         * @brief Construct a new Spawner object
         *
         */
        Spawner(float cooldown = 1.0f, float lastSpawn = 0.0f, float nbSpawned = 0)
            : cooldown(cooldown), lastSpawn(lastSpawn), nbSpawned(nbSpawned) {};
    };
}