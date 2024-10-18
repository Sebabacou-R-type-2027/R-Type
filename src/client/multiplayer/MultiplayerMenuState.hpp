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
#include "../utils/Settings.hpp"
#include <optional>
#include <stdexcept>
#include <string>
#include "../factories/button_factory.hpp"
#include <networkClient/Client.hpp>
#include "../game/GamePlayState.hpp"

namespace rtype {
    class MultiplayerMenuState : public GameState {
    public:
        MultiplayerMenuState(sf::RenderWindow& window, client::Client& network);

        void handleInput() override;
        void update() override;
        void render() override;

    private:
        sf::RenderWindow& window; ///< Reference to the window
        ecs::Registry registry; ///< Registry to store all the components
        ecs::System system; ///< System to store all the systems
        sf::Font font; ///< Font for the text
        sf::Shader backgroundShader; ///< Shader for the background

        std::string hostAddress; ///< Store the host address
        std::string portInput; ///< Store the port input
        std::optional<unsigned int> port; ///< Store the port

        sf::Text addressText; ///< Text for the address
        sf::Text portText; ///< Text for the port
        sf::Text connectButtonText; ///< Text for the connect button


        sf::Vector2f inputRectAddressPos; ///< Position of the address input field
        sf::Vector2f inputRectPortPos; ///< Position of the port input field
        sf::RectangleShape HitboxAddress; ///< Hitbox for the address input field
        sf::RectangleShape HitboxPort; ///< Hitbox for the port input field

        enum InputField { NONE, ADDRESS, PORT } activeField; // Track the active input field

        void initUI(client::Client& network);
    };
}