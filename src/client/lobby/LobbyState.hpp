/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** LobbyState
*/

#ifndef LOBBYSTATE_HPP
#define LOBBYSTATE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../game_state.hpp"
#include "../game/Game.hpp"
#include "../game/GamePlayState.hpp"
#include "../factories/button_factory.hpp"
#include "../utils/FPSCounter.hpp"
#include "../utils/Settings.hpp"

namespace rtype {
    class Game;

    /**
     *  @brief Class that contains the logic of the main menu state
     */
    class LobbyState : public GameState {
    public:
        /**
         * @brief Construct a new Main Menu State object
         *
         * @param window contains the window of the game
         * @param game contains the game object
         */
        LobbyState(sf::RenderWindow& window, Game& game, client::Client& network);
        /**
         * @brief Handle the input of the main menu state
         *
         * @return void
         */

        void handleInput() override;
        /**
         * @brief Update the main menu state
         *
         * @return void
         */
        void update() override;
        /**
         * @brief Render the main menu state
         *
         * @return void
         */
        void render() override;

    private:
        sf::RenderWindow& window; ///< Reference to the window
        Game& game; ///< Reference to the game object

        client::Client& network_;

        sf::Font font; ///< Font for the text
        sf::Shader& backgroundShader; ///< Shader for the background
        ecs::Registry& registry; ///< Registry to store all the components
        ecs::System& system; ///< System to store all the systems
    };
}

#endif // LOBBYSTATE_HPP