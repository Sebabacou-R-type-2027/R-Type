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

        if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Could not load shader");
        }

        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }

        // Initialize view with window size
        gameView = window.getDefaultView();
        gameView.setCenter(sf::Vector2f(gameView.getSize().x / 2, gameView.getSize().y / 2));

        for (int i = 0; i - 1 != network.number_of_players_; i++) {
            initPlayer("assets/Ship/Ship.png", posx * i + 1, true);
        }
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
    auto playerEntities = registry.get_all_player_entity();
    auto& playerPositions = registry.get_components<ecs::Position>();

    for (auto playerEntity : playerEntities) {
        auto& playerPos = playerPositions[static_cast<std::size_t>(playerEntity)];

        if (!playerPos.has_value()) {
            throw std::runtime_error("Player position component not found");
        }

        float viewScrollSpeed = 50.0f * deltaTime;
        playerPos->x += viewScrollSpeed;

        constrainPlayerPosition(playerPos);
    }
}

void GamePlayState::constrainPlayerPosition(std::optional<ecs::Position>& playerPos) {
    float viewLeft = gameView.getCenter().x - gameView.getSize().x / 2;
    float viewRight = gameView.getCenter().x + gameView.getSize().x / 2;
    float viewTop = gameView.getCenter().y - gameView.getSize().y / 2 - 30.0f;
    float viewBottom = gameView.getCenter().y + gameView.getSize().y / 2 - 20.0f;
    float marginX = 10.0f;
    float marginY = 50.0f;

    if (playerPos->x < viewLeft + marginX) {
        playerPos->x = viewLeft + marginX;
    }
    if (playerPos->x > viewRight - marginX - 110.0f) {
        playerPos->x = viewRight - marginX - 110.0f;
    }
    if (playerPos->y < viewTop + marginY - 10) {
        playerPos->y = viewTop + marginY - 10;
    }
    if (playerPos->y > viewBottom - marginY) {
        playerPos->y = viewBottom - marginY;
    }
}



    void GamePlayState::update() {
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system(registry, window, backgroundShader);
        }
        system.control_system(registry, network_);
        system.position_system(registry);

        float deltaTime = calculateDeltaTime();
        bulletSystem.update(registry);
        powerUpSystem.update(registry);
        system.loop_movement_system(registry, deltaTime);
        system.animation_system(registry, deltaTime, window);
        system.collision_system(registry, window);
        handleCollision.handle_collision(registry);
        // // fpsCounter.update();

        moveView(deltaTime); // TODO: Commented out because it break the view movement in network
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

        window.display();
    }

    void GamePlayState::initPlayer(std::string path, float posx, bool me)
    {
        auto player = registry.spawn_entity();
        registry.emplace_component<ecs::Position>(player, 400.0f, posx);
        registry.emplace_component<ecs::Velocity>(player, 0.0f, 0.0f);
        registry.emplace_component<ecs::Drawable>(player, path);
        if (me == true) {
        	registry.emplace_component<ecs::Controllable>(player, true, 5.0f);
        }
        registry.emplace_component<ecs::EntityType>(player, ecs::Type::Player);

        auto& hitbox = registry.emplace_component<ecs::Hitbox>(player, ecs::ShapeType::Rectangle, false, true);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
        hitbox->rect.setOutlineColor(sf::Color::Red);
        hitbox->rect.setOutlineThickness(1.0f);
    }

    void GamePlayState::initChargeBullet()
    {
        auto charge_animation = registry.spawn_entity();
        registry.emplace_component<ecs::Position>(charge_animation, 500.0f, 354.0f);
        registry.emplace_component<ecs::Velocity>(charge_animation, 0.0f, 0.0f);
        registry.emplace_component<ecs::Controllable>(charge_animation, true, 5.0f);
    }
}
