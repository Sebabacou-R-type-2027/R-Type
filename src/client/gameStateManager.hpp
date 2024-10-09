/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_state_manager
*/

#pragma once

#include "game_state.hpp"

namespace rtype {

    class GameStateManager {
    private:
        GameState currentState;

    public:
        GameStateManager() : currentState(GameState::MENU) {}

        void setGameState(GameState newState) {
            currentState = newState;
        }

        GameState getGameState() const { return currentState; }
    };

}
