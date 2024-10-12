/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game
*/

#include "Game.hpp"
#include "../menu/MainMenuState.hpp"
#include "GamePlayState.hpp"

namespace rtype {

    Game::Game(const std::string& title, unsigned int width, unsigned int height, client::Client& network)
        : window(sf::VideoMode(width, height), title), network_(network) {
        window.setFramerateLimit(60);
        currentState = std::make_unique<MainMenuState>(window, *this, network_);
    }

    void Game::run() {
        while (window.isOpen()) {
            currentState->handleInput();
            currentState->update();
            currentState->render();
        }
    }

    void Game::changeState(std::unique_ptr<GameState> newState) {
        currentState = std::move(newState);
    }
}
