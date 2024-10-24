/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GamePlayState
*/

#include "GamePlayState.hpp"
#include "menu/MainMenuState.hpp"
#include <chrono>

namespace rtype {
    GamePlayState::GamePlayState(sf::RenderWindow& window, client::Client& network, bool isSolo, Game& game)
        : window(window), network_(network), isSolo_(isSolo), game(game) {
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
            initPlayer("assets/Player/Spaceship.gif", posx * i + 1, true);
        }
        powerUpSystem.createCardPower(registry, window);
        createEnemies.create_initial_enemies(registry, window);
        initChargeBullet();
    }

    void GamePlayState::handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                game.changeState(std::make_shared<MainMenuState>(window, game, network_));
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
    auto& lifeStates = registry.get_components<ecs::LifeState>();

    for (auto playerEntity : playerEntities) {
        auto& playerPos = playerPositions[static_cast<std::size_t>(playerEntity)];
        auto& lifeState = lifeStates[static_cast<std::size_t>(playerEntity)];

        if (!playerPos.has_value() || !lifeState.has_value()) {
            throw std::runtime_error("Player position component not found");
        }

        if (!lifeState->isAlive) {
            continue;
        }

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


    void GamePlayState::handle_mobs_wave(ecs::Registry& registry, sf::RenderWindow& window) {
        auto& positions = registry.get_components<ecs::Position>();
        auto& lifeStates = registry.get_components<ecs::LifeState>();
        auto enemies = registry.get_all_enemy_entity();
        int check = 0;

        for (auto enemy : enemies) {
            auto& position = positions[static_cast<std::size_t>(enemy)];
            auto& lifeState = lifeStates[static_cast<std::size_t>(enemy)];

            if (!position.has_value() || !lifeState.has_value()) {
                throw std::runtime_error("Enemy position component not found");
            }

            if (lifeState->isAlive) {
                check = 1;
            }

            if (position->x < -100.0f) {
                lifeState->isAlive = false;
            }
        }
        if (check == 0) {
            createEnemies.create_initial_enemies(registry, window);
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
        system.loop_movement_system(registry, deltaTime);
        system.animation_system(registry, deltaTime, window);
        system.shooting_enemy_system(registry, window);
        system.chasing_enemy_system(registry, window);
        system.spawner_enemy_system(registry, window);
        system.collision_system(registry, window);
        handleCollision.handle_collision(registry);

        // moveView(deltaTime);
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
        auto &draw = registry.emplace_component<ecs::Drawable>(player, path);
        draw->sprite.setScale(2.0f, 2.0f);
        if (me == true) {
        	registry.emplace_component<ecs::Controllable>(player, true, 5.0f);
        }
        registry.emplace_component<ecs::EntityType>(player, ecs::Type::Player);
        registry.emplace_component<ecs::CollisionState>(player, false);
        registry.emplace_component<ecs::LifeState>(player, true);

        auto& hitbox = registry.emplace_component<ecs::Hitbox>(player, ecs::ShapeType::Rectangle, false);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(draw->sprite.getGlobalBounds().width, draw->sprite.getGlobalBounds().height));
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
