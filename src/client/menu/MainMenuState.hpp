/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MainMenuState
*/

#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "../game_state.hpp"
#include <SFML/Graphics.hpp>
#include "../game/Game.hpp"

namespace rtype {
    class Game;

    class MainMenuState : public GameState {
    public:
        MainMenuState(sf::RenderWindow& window, Game& game, client::Client& network);

        void handleInput() override;
        void update() override;
        void render() override;

    private:
        sf::RenderWindow& window;
        Game& game;

        void createMenuButtons();
        void startGame();
        sf::Font font;

        ecs::Registry& registry;
        ecs::System& system;

        client::Client& network_;
    };
}

#endif // MAINMENUSTATE_HPP
