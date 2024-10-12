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
#include "../utils/FPSCounter.hpp"

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
        void disableShader();
        sf::Font font;
        sf::Shader& backgroundShader;

        ecs::Registry& registry;
        ecs::System& system;

        bool isShaderEnabled = true;

        // fps counter
        FPSCounter fpsCounter;
    };
}

#endif // MAINMENUSTATE_HPP
