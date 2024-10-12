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
    class Game {
    public:
        Game(const std::string& title, unsigned int width, unsigned int height, client::Client& network);
        void run();
        void changeState(std::unique_ptr<GameState> newState);

        ecs::Registry& getRegistry() { return registry; }
        ecs::System& getSystem() { return system; }
    private:
        sf::RenderWindow window;
        std::unique_ptr<GameState> currentState;

        ecs::Registry registry;
        ecs::System system;

        client::Client& network_;
    };
}
