/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MainMenuState
*/

#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../game_state.hpp"
#include "../game/Game.hpp"
#include "../game/GamePlayState.hpp"
#include "../multiplayer/MultiplayerMenuState.hpp"
#include "../factories/button_factory.hpp"
#include "../utils/FPSCounter.hpp"
#include "../utils/Settings.hpp"

namespace rtype {
    class Game;

    class MainMenuState : public GameState {
    public:
        MainMenuState(sf::RenderWindow& window, Game& game);
        void handleInput() override;
        void update() override;
        void render() override;

    private:
        sf::RenderWindow& window;
        Game& game;

        void createMenuButtons();
        void createMenuTitle();
        void startGame();
        void startMultiplayer();
        void disableShader();
        void enableShader();

        sf::Font font;
        sf::Shader& backgroundShader;

        ecs::Registry& registry;
        ecs::System& system;
    };
}

#endif // MAINMENUSTATE_HPP
