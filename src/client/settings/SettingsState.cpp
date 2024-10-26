/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SettingsState
*/

#include "SettingsState.hpp"

namespace rtype {

        SettingsState::SettingsState(sf::RenderWindow& window, client::Client& network, Game& game)
            : window(window), game(game), registry(game.getRegistry()), system(game.getSystem()), network_(network) {
            if (!font.loadFromFile("assets/fonts/arial.ttf")) {
                throw std::runtime_error("Could not load font");
            }

            if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
                throw std::runtime_error("Could not load shader");
            }

            registry.register_all_components();

            createUI();
        }

        void SettingsState::handleInput() {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    game.changeState(std::make_shared<MainMenuState>(window, game, network_));
                    break;
                }
            }
        }

        void SettingsState::update() {
            if (Settings::getInstance().isShaderEnabled) {
                system.shader_system(registry, window, backgroundShader);
            }
        }

        void SettingsState::createUI() {
            auto settingsBackground = registry.spawn_entity();

            float width = 450.0f * 2;
            float height = 450.0f * 2;

            sf::Color fillColor = sf::Color(100, 100, 100, 255 * 0.7);
            sf::Color outlineColor = sf::Color::Blue;

            registry.emplace_component<ecs::Drawable>(settingsBackground, width, height, fillColor, outlineColor);
            registry.emplace_component<ecs::Position>(settingsBackground, window.getSize().x / 2.0f - width / 2.0f, window.getSize().y / 2.0f - height / 2.0f);


        }

        void SettingsState::render() {
            window.clear();
            if (Settings::getInstance().isShaderEnabled) {
                system.shader_system_render(registry, window, backgroundShader);
            } else {
                sf::Shader::bind(nullptr);
            }

            system.draw_system(registry, window);

            window.display();
        }
}