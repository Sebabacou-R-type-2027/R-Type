/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MultiplayerMenuState
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "game_state.hpp"
#include "registry.hpp"
#include "system.hpp"
#include <optional>
#include <networkClient/Client.hpp>
#include "menu/MainMenuState.hpp"
#include <string>
#include "../factories/button_factory.hpp"
#include <networkClient/Client.hpp>
#include "../game/GamePlayState.hpp"
#include "../game/Game.hpp"

namespace rtype {
    class MultiplayerMenuState : public GameState {
    public:
        MultiplayerMenuState(sf::RenderWindow& window, client::Client& network, Game& game);

        void handleInput() override;
        void update() override;
        void render() override;

    private:
        sf::RenderWindow& window; ///< Reference to the window
        ecs::Registry registry; ///< Registry to store all the components
        ecs::System system; ///< System to store all the systems
        sf::Font font; ///< Font for the text
        sf::Shader backgroundShader; ///< Shader for the background
        Game &game; ///< Reference to the game object
        client::Client& network_; ///< Reference to the network object

        std::string hostAddress; ///< Store the host address
        std::string portInput; ///< Store the port input
        std::string username; ///< Store the username
        std::string password; ///< Store the password
        std::string secretpassword; ///< Store the password hash
        std::optional<unsigned int> port; ///< Store the port

        sf::Text addressText; ///< Text for the address
        sf::Text portText; ///< Text for the port
        sf::Text usernameText; ///< Text for the username
        sf::Text passwordText; ///< Text for the password
        sf::Text connectButtonText; ///< Text for the connect button


        sf::Vector2f inputRectAddressPos; ///< Position of the address input field
        sf::Vector2f inputRectPortPos; ///< Position of the port input field
        sf::RectangleShape HitboxAddress; ///< Hitbox for the address input field
        sf::RectangleShape HitboxPort; ///< Hitbox for the port input field
        bool showPassword = false; ///< Show the password

        enum InputField {ADDRESS, PORT, USERNAME, PASSWORD} activeField; // Track the active input field

        void startLobby();
        void initUI(client::Client& network);
    };
}