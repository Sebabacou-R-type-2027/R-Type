/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ennemy_state_system
*/

#include "ennemy_state_system.hpp"

namespace ecs::systems {

    void EnemyStateSystem::update(Registry& registry, float deltaTime) {
        auto states = registry.get_components<EnemyState>();
        auto positions = registry.get_components<Position>();
        auto velocities = registry.get_components<Velocity>();

        for (std::size_t i = 0; i < states.size(); ++i) {
            if (states[i]) {
                EnemyState &state = *states[i];

                switch (state.current_state) {
                    case State::Idle:
                        velocities[i]->get().vx = 0;
                        velocities[i]->get().vy = 0;
                        break;

                    case State::Attacking:
                        velocities[i]->get().vx = 100.0f;
                        velocities[i]->get().vy = 100.0f;
                        break;

                    case State::Patrolling:
                        velocities[i]->get().vx = 50.0f;
                        velocities[i]->get().vy = 0.0f;
                        break;
                }
            }
        }
    }
}