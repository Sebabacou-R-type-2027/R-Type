/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PowerUp_state
*/


#pragma once

namespace ecs {


    enum class State{
        Idle,
        Patrolling };

    struct PowerUpState {
        State current_state;

        PowerUpState(State state = State::Idle)
        : current_state(state) {}
    };
}
