/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game
*/

#include "Game.hpp"
#include "../menu/MainMenuState.hpp"
#include "GamePlayState.hpp"
#include "Sound_game.hpp"

namespace rtype {

    Game::Game(const std::string& title, unsigned int width, unsigned int height, client::Client& network)
        : window(sf::VideoMode(width, height), title), network_(network) {
        window.setFramerateLimit(60);
        currentState = std::make_unique<MainMenuState>(window, *this, network_);
        if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Could not load shader");
        }
    }

    void Game::run() {
        Sound_game.playMusic("assets/Son/music.wav", 50, true);

        while (window.isOpen()) {
            if (currentState == nullptr) {
                window.close();
                network_.is_running_ = true;
                return;
            }
            currentState->handleInput();
            currentState->update();
            currentState->render();
        }
        network_.is_running_ = true;
    }

    void Game::changeState(std::shared_ptr<GameState> newState) {
        currentState = newState;
    }
}
