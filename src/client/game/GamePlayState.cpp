/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GamePlayState
*/

#include "GamePlayState.hpp"
#include <iostream>
#include <chrono>

namespace rtype {
    GamePlayState::GamePlayState(sf::RenderWindow& window, client::Client& network)
        : window(window), network_(network) {
        int posx = 200;

        registry.register_all_components();
        std::cout << "nbr player to load " << network.number_of_players_ << std::endl;
        for (int i = 0; i - 1 != network.number_of_players_; i++) {
            initPlayer("assets/Ship/Ship.png", posx * i + 1);
        }
        createEnnemies.create_enemies(registry, window);

        // initChargeBullet();
    }

    void GamePlayState::handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

        }
    }

    void GamePlayState::update() {
        system.control_system(registry, network_);
        system.position_system(registry);

        auto currentTime = std::chrono::high_resolution_clock::now();
        static auto lastTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;

        system.bullet_system(registry);
        system.loop_movement_system(registry, deltaTime);
        system.collision_system(registry, window);

    }

    void GamePlayState::render() {
        window.clear();
        system.draw_system(registry, window);
        window.display();
    }

    void GamePlayState::initPlayer(std::string path, float posx)
    {
        auto player = registry.spawn_entity();
        registry.emplace_component<ecs::Position>(player, 400.0f, posx);
        registry.emplace_component<ecs::Velocity>(player, 0.0f, 0.0f);
        registry.emplace_component<ecs::Drawable>(player, path);
        registry.emplace_component<ecs::Controllable>(player, true, 5.0f);
        registry.emplace_component<ecs::EntityType>(player, ecs::Type::Player);

        auto& hitbox = registry.emplace_component<ecs::Hitbox>(player, ecs::ShapeType::Rectangle, false);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
    }

    void GamePlayState::initChargeBullet()
    {
        auto charge_animation = registry.spawn_entity();
        registry.emplace_component<ecs::Position>(charge_animation, 500.0f, 354.0f);
        registry.emplace_component<ecs::Velocity>(charge_animation, 0.0f, 0.0f);
        registry.emplace_component<ecs::Controllable>(charge_animation, true, 5.0f);
    }
}
