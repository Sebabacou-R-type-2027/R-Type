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
#include "bullet_event.hpp"
#include <SFML/Audio.hpp>



namespace rtype {
    Game::Game(const std::string& title, unsigned int width, unsigned int height, const std::string& playerTexturePath)
        : window(sf::VideoMode(width, height), title) {
        window.setFramerateLimit(60);
    }

    void Game::initChargeBullet()
    {
        auto charge_animation = registry.spawn_entity();
        registry.emplace_component<ecs::Position>(charge_animation, 500.0f, 354.0f);
        registry.emplace_component<ecs::Velocity>(charge_animation, 0.0f, 0.0f);
        registry.emplace_component<ecs::Controllable>(charge_animation, true, 5.0f);
        registry.emplace_component<ecs::Acceleration>(charge_animation, 0.0f, 0.0f);
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

        initChargeBullet();

        sf::Font font;
        font.loadFromFile("assets/fonts/NimbusSanL-Bol.otf");


        while (window.isOpen()) {
            processEvents();
            rtype::game::Sound_game Sound_game;
            Sound_game.playSound("assets/Son/laser_gun1.wav");

            update(bulletSystem);
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

    void Game::update(rtype::game::BulletSystem& bulletSystem) {
        system.control_system(registry);
        system.position_system(registry);
        system.button_system(registry, window);
        system.collision_system(registry, window);
        bulletSystem.update(registry);
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
