/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GamePlayState
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "game_state.hpp"
#include "registry.hpp"
#include "system.hpp"

namespace rtype {
    class GamePlayState : public GameState {
    public:
        GamePlayState(sf::RenderWindow& window);

        void handleInput() override;
        void update() override;
        void render() override;

    private:
        sf::RenderWindow& window;
        ecs::Registry registry;
        ecs::System system;
    };
}
