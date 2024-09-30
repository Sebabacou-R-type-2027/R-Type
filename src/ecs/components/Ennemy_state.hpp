/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Ennemy_state
*/

#pragma once

namespace ecs {
    enum class State{
        Idle,
        Attacking,
        Patrolling };

    struct EnemyState {
        State current_state;

        EnemyState(State state = State::Idle)
        : current_state(state) {}
    };
}
