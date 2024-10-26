/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SettingsState
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "../game_state.hpp"
#include "../game/Game.hpp"
#include "utils/Settings.hpp"
#include "../factories/button_factory.hpp"
#include "../menu/MainMenuState.hpp"

namespace rtype {
    class Game;

    /**
     *  @brief Class that contains the logic of the main menu state
     */
    class SettingsState : public GameState {
        public:
            SettingsState(sf::RenderWindow& window, client::Client& network, Game& game);

            void handleInput() override;
            void update() override;
            void render() override;

        private:
            sf::RenderWindow& window; ///< Reference to the window
            Game& game; ///< Reference to the game object
            client::Client& network_;

            void createUI();

            sf::Font font; ///< Font for the text
            sf::Shader backgroundShader; ///< Shader for the background
            ecs::Registry registry; ///< Registry to store all the components
            ecs::System system; ///< System to store all the systems
    };
}