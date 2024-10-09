/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameState
*/

#pragma once

namespace ecs {
    enum GameState {
        MENU,
        GAME,
        PAUSE,
        GAMEOVER
    };

    struct GameStateComponent
    {
        GameState current_state;

        GameStateComponent(GameState state = GameState::MENU)
            : current_state(state) {}
    };
};
