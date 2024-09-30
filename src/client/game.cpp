/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#include "game.hpp"
#include <iostream>
#include "button_factory.hpp"
#include <chrono>

namespace rtype {
    Game::Game(const std::string& title, unsigned int width, unsigned int height, const std::string& playerTexturePath)
        : window(sf::VideoMode(width, height), title) {
        window.setFramerateLimit(60);
    }

    void Game::run() {
        registry.register_all_components();

        auto movable_entity = registry.spawn_entity();
        registry.emplace_component<ecs::Position>(movable_entity, 400.0f, 300.0f);
        registry.emplace_component<ecs::Velocity>(movable_entity, 0.0f, 0.0f);
        registry.emplace_component<ecs::Drawable>(movable_entity, "assets/Ship/Ship.png");
        registry.emplace_component<ecs::Controllable>(movable_entity, true, 5.0f);
        registry.emplace_component<ecs::Acceleration>(movable_entity, 0.0f, 0.0f);
        registry.emplace_component<ecs::EntityType>(movable_entity, ecs::Type::Player);
        auto& hitbox = registry.emplace_component<ecs::Hitbox>(movable_entity, ecs::ShapeType::Rectangle, false);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));

        createEnnemies.create_enemies(registry, window);

        sf::Font font;
        font.loadFromFile("assets/fonts/NimbusSanL-Bol.otf");

        auto button_entity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            button_entity,
            ecs::ButtonFactory::create_button(
                "Click me!",
                sf::Vector2f(400.0f, 400.0f),
                sf::Vector2f(200.0f, 50.0f),
                font,
                sf::Color::Blue,      // Set default button color
                sf::Color::Cyan,     // Set hover color
                sf::Color::Green,     // Set click color
                24,                   // Set text size
                []() { std::cout << "Button clicked!" << std::endl; } // Click action
            )
        );
        registry.emplace_component<ecs::EntityType>(button_entity, ecs::Type::Button);


        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

    void Game::processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }
    }

    void Game::update() {
        system.control_system(registry);
        system.position_system(registry);
        system.button_system(registry, window);
        system.collision_system(registry, window);
        auto currentTime = std::chrono::high_resolution_clock::now();
        static auto lastTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;
        system.loop_movement_system(registry, deltaTime);

        auto& positions = registry.get_components<ecs::Position>();
        auto& drawables = registry.get_components<ecs::Drawable>();
        auto& entities = registry.get_components<ecs::EntityType>();

        for (std::size_t i = 0; i < positions.size(); ++i) {
            if (positions[i] && drawables[i] && entities[i]) { // Ensure both components exist  
                if (positions[i]->x < 0) positions[i]->x = 0;
                if (positions[i]->x > window.getSize().x - drawables[i]->size) // Use size from Drawable
                    positions[i]->x = window.getSize().x - drawables[i]->size;
                if (positions[i]->y < 0) positions[i]->y = 0;
                if (positions[i]->y > window.getSize().y - drawables[i]->size)
                    positions[i]->y = window.getSize().y - drawables[i]->size;
            }
        }
    }

    void Game::render() {
        window.clear();
        system.draw_system(registry, window); // Update to just pass the registry and window
        system.button_system_render(registry, window);
        window.display();
    }
}
