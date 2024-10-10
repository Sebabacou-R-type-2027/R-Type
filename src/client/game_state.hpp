/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_state
*/

#pragma once

namespace rtype {

    class GameState {
    public:
        virtual ~GameState() = default;

        virtual void handleInput() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
    };
}
