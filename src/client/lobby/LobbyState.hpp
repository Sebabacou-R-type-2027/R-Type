/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** LobbyState
*/

#ifndef LOBBYSTATE_HPP
#define LOBBYSTATE_HPP

#include <SFML/Graphics.hpp>
#include "../game_state.hpp"
#include "../game/Game.hpp"
#include "utils/Settings.hpp"
#include "../factories/button_factory.hpp"

namespace rtype {
    class Game;

    /**
     *  @brief Class that contains the logic of the main menu state
     */
    class LobbyState : public GameState {
        public:
            LobbyState(sf::RenderWindow& window, client::Client& network, Game& game);

            void handleInput() override;
            void update() override;
            void render() override;

        private:
            sf::RenderWindow& _window; ///< Reference to the window
            Game& game; ///< Reference to the game object

            client::Client& network_;

            std::string CreateLobbyInput; ///< Store the host CreateLobby
            std::string JoinLobbyInput; ///< Store the JoinLobby input

            sf::Text CreateLobbyText; ///< Text for the CreateLobby
            sf::Text JoinLobbyText; ///< Text for the JoinLobby


            sf::Font font; ///< Font for the text
            sf::Shader backgroundShader; ///< Shader for the background
            ecs::Registry registry; ///< Registry to store all the components
            ecs::System system; ///< System to store all the systems

            enum InputField {CREATE, JOIN} activeField = CREATE; ///< Track the active input field

            void initUI(client::Client& network);
            ecs::Drawable* inputRectCreateLobbyDrawablePtr;
            ecs::Drawable* inputRectJoinLobbyDrawablePtr;
    };
}

#endif // LOBBYSTATE_HPP
