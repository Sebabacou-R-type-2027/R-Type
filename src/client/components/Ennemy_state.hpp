/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Ennemy_state
*/

#pragma once

namespace ecs {

    /**
     * @brief Class that contains the state of an enemy
     */
    enum class State{
        Idle,
        Attacking,
        Patrolling };
    /**
     * @brief Class that contains the state of an enemy
     */
    struct EnemyState {
        State current_state;
        /**
         * @brief Construct a new Enemy State object
         * 
         * @param state contains the state of the enemy
         */
        EnemyState(State state = State::Idle)
        : current_state(state) {}
    };
}
