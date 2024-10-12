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
#include <networkClient/Client.hpp>

#include "../ecs/registry.hpp"
#include "../ecs/system.hpp"

namespace rtype {
    class GameState;

    class Game {
    public:
        Game(const std::string& title, unsigned int width, unsigned int height, client::Client& network);
        void run();
        void changeState(std::shared_ptr<GameState> newState);

        ecs::Registry& getRegistry() { return registry; }
        ecs::System& getSystem() { return system; }
        sf::Shader& getBackgroundShader() { return backgroundShader; }

    private:
        sf::RenderWindow window;
        std::shared_ptr<GameState> currentState;
        sf::Shader backgroundShader;

        ecs::Registry registry;
        ecs::System system;

        client::Client& network_;
    };
}
