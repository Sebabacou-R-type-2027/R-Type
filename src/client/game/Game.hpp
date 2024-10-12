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
#include "../ecs/registry.hpp"
#include "../ecs/system.hpp"

namespace rtype {
    class Game {
    public:
        Game(const std::string& title, unsigned int width, unsigned int height);
        void run();
        void changeState(std::unique_ptr<GameState> newState);

        ecs::Registry& getRegistry() { return registry; }
        ecs::System& getSystem() { return system; }
        sf::Shader& getBackgroundShader() { return backgroundShader; }
    private:
        sf::RenderWindow window;
        std::unique_ptr<GameState> currentState;
        sf::Shader backgroundShader;

        ecs::Registry registry;
        ecs::System system;
    };
}
