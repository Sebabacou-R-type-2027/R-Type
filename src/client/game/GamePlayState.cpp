/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GamePlayState
*/

#include "GamePlayState.hpp"
#include <iostream>
#include <chrono>

namespace rtype {
    GamePlayState::GamePlayState(sf::RenderWindow& window)
        : window(window) {
        registry.register_all_components();
    }

    void GamePlayState::handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

        }
    }

    void GamePlayState::update() {
        system.control_system(registry);
        system.position_system(registry);

        auto currentTime = std::chrono::high_resolution_clock::now();
        static auto lastTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;

        system.bullet_system(registry);
        system.loop_movement_system(registry, deltaTime);
        system.collision_system(registry, window);

    }

    void GamePlayState::render() {
        window.clear();
        system.draw_system(registry, window);
        window.display();
    }
}
