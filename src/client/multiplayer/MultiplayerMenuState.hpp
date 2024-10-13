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

namespace rtype {
    class MultiplayerMenuState : public GameState {
    public:
        MultiplayerMenuState(sf::RenderWindow& window);

        void handleInput() override;
        void update() override;
        void render() override;

    private:
        sf::RenderWindow& window;
        ecs::Registry registry;
        ecs::System system;
        sf::Font font;
        sf::Shader backgroundShader;

        std::string hostAddress;
        std::string portInput; // Store raw input for port
        std::optional<unsigned int> port; // Store validated port

        sf::Text addressText;
        sf::Text portText;
        sf::Text connectButtonText;


        sf::Vector2f inputRectAddressPos;
        sf::Vector2f inputRectPortPos;
        sf::RectangleShape HitboxAddress;
        sf::RectangleShape HitboxPort;

        enum InputField { NONE, ADDRESS, PORT } activeField; // Track the active input field

        void initUI();
    };
}