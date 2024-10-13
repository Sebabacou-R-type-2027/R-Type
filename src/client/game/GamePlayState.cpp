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
    GamePlayState::GamePlayState(sf::RenderWindow& window)
        : window(window) {
        registry.register_all_components();

        if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Could not load shader");
        }

        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }

        // Initialize view with window size
        gameView = window.getDefaultView();
        gameView.setCenter(sf::Vector2f(gameView.getSize().x / 2, gameView.getSize().y / 2));

        initPlayer("assets/Ship/Ship.png");
        createEnnemies.create_enemies(registry, window);
        initChargeBullet();
    }


    void GamePlayState::handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

        }
    }

    float GamePlayState::calculateDeltaTime() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        static auto lastTime = currentTime;
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        return deltaTime;
    }

    void GamePlayState::moveView(float deltaTime) {
        float viewScrollSpeed = 50.0f * deltaTime;
        gameView.move(viewScrollSpeed, 0.0f);
    }

    void GamePlayState::handlePlayerMovement(float deltaTime) {
        auto playerEntity = registry.get_player_entity();
        auto& playerPositions = registry.get_components<ecs::Position>();
        auto& playerPos = playerPositions[static_cast<std::size_t>(playerEntity)];

        if (!playerPos.has_value()) {
            throw std::runtime_error("Player position component not found");
        }

        float viewScrollSpeed = 50.0f * deltaTime;
        playerPos->x += viewScrollSpeed;

        constrainPlayerPosition(playerPos);
    }

    void GamePlayState::constrainPlayerPosition(std::optional<ecs::Position>& playerPos) {
        float viewLeft = gameView.getCenter().x - gameView.getSize().x / 2;
        float viewRight = gameView.getCenter().x + gameView.getSize().x / 2;
        float viewTop = gameView.getCenter().y - gameView.getSize().y / 2;
        float viewBottom = gameView.getCenter().y + gameView.getSize().y / 2;

        float marginX = 50.0f;
        float marginY = 50.0f;

        if (playerPos->x < viewLeft + marginX) {
            playerPos->x = viewLeft + marginX;
        }
        if (playerPos->x > viewRight - marginX) {
            playerPos->x = viewRight - marginX;
        }
        if (playerPos->y < viewTop + marginY) {
            playerPos->y = viewTop + marginY;
        }
        if (playerPos->y > viewBottom - marginY) {
            playerPos->y = viewBottom - marginY;
        }
    }

    void GamePlayState::update() {
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system(registry, window, backgroundShader);
        }

        system.control_system(registry);
        system.position_system(registry);

        float deltaTime = calculateDeltaTime();
        bulletSystem.update(registry);
        system.loop_movement_system(registry, deltaTime);
        system.collision_system(registry, window);

        moveView(deltaTime);
        handlePlayerMovement(deltaTime);

        window.setView(gameView);
    }



    void GamePlayState::render() {
        window.clear();

        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system_render(registry, window, backgroundShader);
        } else {
            sf::Shader::bind(nullptr);
        }

        window.setView(gameView);

        system.draw_system(registry, window);

        sf::RectangleShape topBorder(sf::Vector2f(window.getSize().x, 5.0f));
        topBorder.setPosition(0, 50);
        topBorder.setFillColor(sf::Color::White);

        sf::RectangleShape bottomBorder(sf::Vector2f(window.getSize().x, 5.0f));
        bottomBorder.setPosition(0, window.getSize().y - 55);
        bottomBorder.setFillColor(sf::Color::White);

        window.draw(topBorder);
        window.draw(bottomBorder);

        window.display();
    }




    void GamePlayState::initPlayer(std::string path)
    {
        auto player = registry.spawn_entity();
        registry.emplace_component<ecs::Position>(player, 400.0f, 300.0f);
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
