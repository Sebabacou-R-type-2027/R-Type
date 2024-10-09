/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MainMenuState
*/

#include "MainMenuState.hpp"
#include "../game/Game.hpp"

namespace rtype {
    MainMenuState::MainMenuState(sf::RenderWindow& window, Game& game)
        : window(window), game(game) {
    }

    void MainMenuState::handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }
    }

    void MainMenuState::update() {
    }

    void MainMenuState::render() {
        window.clear();

        window.display();
    }

}
