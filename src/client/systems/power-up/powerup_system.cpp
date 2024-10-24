/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#include "powerup_system.hpp"
#include <iostream>
#include "Sound_game.hpp"
#include <SFML/Audio.hpp>
#include "components/Entity_type.hpp"
#include "utils/CheckEntity.hpp"
#include "../factories/button_factory.hpp"

namespace rtype::game {


void PowerUp::createCardPower(ecs::Registry& registry, sf::RenderWindow& window) {
    if (isCreate == false) {
        isCreate = true;
        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }
        sf::Vector2u window_size = window.getSize();
        float button_height = 50.0f;
        float spacing = 20.0f;
        float total_height = (button_height + spacing) * 7;
        float start_y = (window_size.y - total_height) / 2.0f;

        auto createButton = [this, button_height, &window, &registry](const std::string& label, float yPos, std::function<void()> callback) -> ecs::Entity {
            sf::Text text(label, font, 24);
            float button_width = text.getLocalBounds().width + 40.0f; // Add padding to the text width
            float button_x = (window.getSize().x - button_width) / 2.0f; // Center the button

            button_x += button_width / 2.0f;

            auto entity = registry.spawn_entity();
            registry.emplace_component<ecs::Button>(entity,
                ecs::ButtonFactory::create_button(
                    label,
                    sf::Vector2f(button_x, yPos),
                    sf::Vector2f(button_width, button_height),
                    font,
                    sf::Color(14, 94, 255, 255),
                    sf::Color(7, 115, 255, 255),
                    sf::Color::Blue,
                    sf::Color::White,
                    24,
                    callback
                )
            );
            return entity;
        };

        float yPos = start_y;
        createButton("AUGUSTIN 1", yPos, [this]() { return; });
        yPos += button_height + spacing;

        createButton("AUGUSTIN 2", yPos, [this]() { return; });
        yPos += button_height + spacing;

        createButton("AUGUSTIN 3", yPos, [this]() { return; });
        yPos += button_height + spacing;
        }
    }


    bool PowerUp::threeShootPower(ecs::Registry& registry) {
        if (!threeShootPower) {
            threeShootPower = true; // Activer le pouvoir
            timer.restart(); // Réinitialiser le timer
            std::cout << "Power activated for 15 seconds!" << std::endl;
        }

        // Vérifiez si 15 secondes se sont écoulées
        if (threeShootPower && timer.getElapsedTime().asSeconds() >= 15.0f) {
            threeShootPower = false; // Désactiver le pouvoir
            std::cout << "Power deactivated." << std::endl;
        }

        return;
    }
}

