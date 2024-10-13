/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameState
*/

#pragma once

namespace ecs {
    /**
     * @brief Class that contains the state of the game
     */
    enum GameState {
        MENU,
        GAME,
        PAUSE,
        GAMEOVER
    };

    /**
     * @brief Class that contains the state of the game
     */
    struct GameStateComponent
    {
        GameState current_state; ///< Current state of the game

        /**
         * @brief Construct a new Game State Component object
         * 
         * @param state contains the state of the game
         */
        GameStateComponent(GameState state = GameState::MENU)
            : current_state(state) {}
    };
};
