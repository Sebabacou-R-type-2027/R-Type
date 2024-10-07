/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#include "Game.hpp"
#include <iostream>
#include <chrono>

namespace rtype {
    Game::Game(const std::string& title, unsigned int width, unsigned int height, const std::string& playerTexturePath)
        : window(sf::VideoMode(width, height), title) {
        window.setFramerateLimit(60);
    }

    void Game::run() {
        registry.register_all_components();

        sf::Font font;
        if (!font.loadFromFile("assets/fonts/NimbusSanL-Bol.otf")) {
            std::cerr << "Failed to load font!" << std::endl;
            return;
        }

        fpsText.setFont(font);
        fpsText.setCharacterSize(24);
        fpsText.setFillColor(sf::Color::White);
        fpsText.setPosition(10.0f, 10.0f);


        if (gameStateManager.getGameState() == GameState::MENU) {
            createMenuButtons(font);
        }

        while (window.isOpen()) {
            processEvents(gameStateManager);

            switch (gameStateManager.getGameState()) {
                case GameState::MENU:
                    update();
                    renderMenu();
                    break;
                case GameState::GAME:
                    update();
                    render();
                    break;
                case GameState::PAUSE:
                    renderPauseScreen();
                    break;
                case GameState::GAMEOVER:
                    renderGameOverScreen();
                    break;
            }

            fpsCounter.update();
        }
    }

    void Game::processEvents(GameStateManager& gameStateManager) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }
    }



    void Game::update() {
        system.control_system(registry);
        system.position_system(registry);
        if (gameStateManager.getGameState() == GameState::MENU)
            system.button_system(registry, window);
        if (gameStateManager.getGameState() == GameState::GAME) {
            system.collision_system(registry, window);
            auto currentTime = std::chrono::high_resolution_clock::now();
            static auto lastTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
            lastTime = currentTime;
            system.bullet_system(registry);
            system.loop_movement_system(registry, deltaTime);
        }

        system.animation_system(registry, deltaTime, window);
        handleCollision.handle_collision(registry);
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
        system.draw_system(registry, window);
        fpsText.setString("FPS: " + std::to_string(fpsCounter.getFPS()));
        window.draw(fpsText);
        window.display();
    }

    void Game::renderMenu() {
        window.clear();
        system.draw_system(registry, window);
        system.button_system_render(registry, window);
        window.display();
    }

    void Game::renderPauseScreen() {
        window.clear();
        window.display();
    }

    void Game::renderGameOverScreen() {
        window.clear();
        window.display();
    }

    void Game::renderGame() {
        window.clear();
        window.display();
    }

    void Game::createMenuButtons(sf::Font& font) {
        sf::Vector2u window_size = window.getSize(); // Get the current window size
        float button_width = 200.0f;
        float button_x = (window_size.x - button_width) / 2.0f;  // Dynamic centered X position

        auto SoloCampaignButtonEntity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            SoloCampaignButtonEntity,
            ecs::ButtonFactory::create_button(
                "Solo Campaign",
                sf::Vector2f(button_x, 300.0f),
                sf::Vector2f(button_width, 50.0f),
                font,
                sf::Color::Blue,
                sf::Color::Cyan,
                sf::Color::Green,
                sf::Color::White,
                24,
                [this]() { startGame(); }
            )
        );

        auto MultiplayerButtonEntity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            MultiplayerButtonEntity,
            ecs::ButtonFactory::create_button(
                "Multiplayer",
                sf::Vector2f(button_x, 400.0f),
                sf::Vector2f(button_width, 50.0f),
                font,
                sf::Color::Blue,
                sf::Color::Cyan,
                sf::Color::Green,
                sf::Color::White,
                24,
                [this]() { startGame(); }
            )
        );

        auto CustomizeShipButtonEntity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            CustomizeShipButtonEntity,
            ecs::ButtonFactory::create_button(
                "Customize Ship",
                sf::Vector2f(button_x, 500.0f),
                sf::Vector2f(button_width, 50.0f),
                font,
                sf::Color::Blue,
                sf::Color::Cyan,
                sf::Color::Green,
                sf::Color::White,
                24,
                [this]() { startGame(); }
            )
        );

        auto SettingsButtonEntity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            SettingsButtonEntity,
            ecs::ButtonFactory::create_button(
                "Settings",
                sf::Vector2f(button_x, 600.0f),
                sf::Vector2f(button_width, 50.0f),
                font,
                sf::Color::Blue,
                sf::Color::Cyan,
                sf::Color::Green,
                sf::Color::White,
                24,
                [this]() { startGame(); }
            )
        );

        auto MapEditorButtonEntity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            MapEditorButtonEntity,
            ecs::ButtonFactory::create_button(
                "Map Editor",
                sf::Vector2f(button_x, 700.0f),
                sf::Vector2f(button_width, 50.0f),
                font,
                sf::Color::Blue,
                sf::Color::Cyan,
                sf::Color::Green,
                sf::Color::White,
                24,
                [this]() { startGame(); }
            )
        );

        auto GameOptionsButtonEntity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            GameOptionsButtonEntity,
            ecs::ButtonFactory::create_button(
                "Game Options",
                sf::Vector2f(button_x, 800.0f),
                sf::Vector2f(button_width, 50.0f),
                font,
                sf::Color::Blue,
                sf::Color::Cyan,
                sf::Color::Green,
                sf::Color::White,
                24,
                [this]() { startGame(); }
            )
        );

        auto GameQuitButtonEntity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            GameQuitButtonEntity,
            ecs::ButtonFactory::create_button(
                "Quit",
                sf::Vector2f(button_x, 900.0f),
                sf::Vector2f(button_width, 50.0f),
                font,
                sf::Color::Blue,
                sf::Color::Cyan,
                sf::Color::Green,
                sf::Color::White,
                24,
                [this]() { window.close(); }
            )
        );
    }

    void Game::startGame() {
        gameStateManager.setGameState(GameState::GAME);

        auto movable_entity = registry.spawn_entity();
        registry.emplace_component<ecs::Position>(movable_entity, 400.0f, 300.0f);
        registry.emplace_component<ecs::Velocity>(movable_entity, 0.0f, 0.0f);
        registry.emplace_component<ecs::Drawable>(movable_entity, "assets/Ship/Ship.png");
        registry.emplace_component<ecs::Controllable>(movable_entity, true, 5.0f);
        registry.emplace_component<ecs::EntityType>(movable_entity, ecs::Type::Player);
        registry.emplace_component<ecs::CollisionState>(movable_entity, false);
        auto& hitbox = registry.emplace_component<ecs::Hitbox>(movable_entity, ecs::ShapeType::Rectangle, false);
        hitbox->rect = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));

        createEnnemies.create_enemies(registry, window);

        initChargeBullet();

        std::cout << "Game started!" << std::endl;
    }

    void Game::initChargeBullet()
    {
        auto charge_animation = registry.spawn_entity();
        registry.emplace_component<ecs::Position>(charge_animation, 500.0f, 354.0f);
        registry.emplace_component<ecs::Velocity>(charge_animation, 0.0f, 0.0f);
        registry.emplace_component<ecs::Controllable>(charge_animation, true, 5.0f);
    }
}


