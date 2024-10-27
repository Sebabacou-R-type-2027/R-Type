/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_system
*/

#include "powerup_system.hpp"
#include "../utils/CheckEntity.hpp"
#include "system.hpp"
#include <iostream>
#include <memory>

namespace rtype {


PowerUp::PowerUp(ecs::Registry& registry, sf::RenderWindow& window): registry(registry), window(window) {
    if (isCreate == false) {
        isCreate = true;
        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }
        sf::Vector2u window_size = window.getSize();
        float button_height = 50.0f;
        float spacing = 160.0f;
        float total_height = (button_height + spacing) * 7;
        float start_y = (window_size.x - total_height) / 2.0f;

        auto createButton = [this, button_height, &window, &registry](const std::string& label, float yPos, std::function<void()> callback) -> ecs::Entity {
            sf::Text text(label, font, 24);
            float button_width = text.getLocalBounds().width + 40.0f; // Add padding to the text width
            float button_x = (window.getSize().x - button_width) / 2.0f; // Center the button

            button_x += button_width / 2.0f;

            auto entity = registry.spawn_entity();
            registry.emplace_component<ecs::Button>(entity,
                ecs::ButtonFactory::create_button(
                    label,
                    sf::Vector2f(yPos, button_x),
                    sf::Vector2f(button_width, button_height),
                    font,
                    sf::Color(14, 94, 255, 255),
                    sf::Color(7, 115, 255, 255),
                    sf::Color::Yellow,
                    sf::Color::White,
                    24,
                    callback
                )
            );
            return entity;
        };

        float yPos = start_y;
        createButton("POWER 1", yPos, [this, &registry]() { printf("COUCOU");});
        yPos += button_height + spacing;

        createButton("AUGUSTIN 2", yPos, [this]() { return; });
        yPos += button_height + spacing;

        createButton("AUGUSTIN 3", yPos, [this]() { return; });
        yPos += button_height + spacing;
        }
    }


    //bool PowerUp::threeShootPower(ecs::Registry& registry) {
    //    if (!threeShootPower_activate) {
    //        threeShootPower_activate = true;
    //        timer.restart();
    //        std::cout << "Power activated for 15 seconds!" << std::endl;
    //    }

    //    if (threeShootPower_activate && timer.getElapsedTime().asSeconds() >= 15.0f) {
    //        threeShootPower_activate = false;
    //        std::cout << "Power deactivated." << std::endl;
    //    }

    //    return threeShootPower_activate;
    //}


    void PowerUp::update() {
        system.button_system(registry, window);
        // fpsCounter.update();
    }

    void PowerUp::handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    void PowerUp::render() {
        window.clear();
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system_render(registry, window, backgroundShader);
        } else {
            sf::Shader::bind(nullptr);
        }

        system.button_system_render(registry, window);
        system.draw_system(registry, window);

        // Draw FPS counter
        // sf::Text fpsText("FPS: " + std::to_string(fpsCounter.getFPS()), font, 24);
        // window.draw(fpsText);

        window.display();
        }
}
