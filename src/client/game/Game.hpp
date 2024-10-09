/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "game_state.hpp"
#include <memory>

namespace rtype {
    class Game {
    public:
        Game(const std::string& title, unsigned int width, unsigned int height, const std::string& playerTexturePath);
        void run();
        void changeState(std::unique_ptr<GameState> newState);
    private:
        sf::RenderWindow window;
        std::unique_ptr<GameState> currentState;
    };
}
