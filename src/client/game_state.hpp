/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_state
*/

#pragma once

namespace rtype {

    /**
     * @brief Class that contains the state of the game
     */
    class GameState {
    public:
        virtual ~GameState() = default;

        virtual void handleInput() = 0; ///< Handle the input of the game
        virtual void update() = 0; ///< Update the game
        virtual void render() = 0; ///< Render the game
    };
}
