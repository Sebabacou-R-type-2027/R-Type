/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#include "game.hpp"
#include <iostream>
#include "button_factory.hpp"
#include "ecs/entity.hpp"

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

        if (gameStateManager.getGameState() == GameState::MENU)
            createMenuButtons(font);

        if (GameState::GAME) { // add space ship

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
        system.loop_movement_system(registry);
        if (gameStateManager.getGameState() == GameState::MENU)
            system.button_system(registry, window);


        auto& positions = registry.get_components<ecs::Position>();
        auto& drawables = registry.get_components<ecs::Drawable>();

        for (std::size_t i = 0; i < positions.size(); ++i) {
            if (positions[i] && drawables[i]) { // Ensure both components exist
                // Boundary checks for positions
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
        auto startButtonEntity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            startButtonEntity,
            ecs::ButtonFactory::create_button(
                "Start Game",
                sf::Vector2f(400.0f, 300.0f),
                sf::Vector2f(200.0f, 50.0f),
                font,
                sf::Color::Blue,
                sf::Color::Cyan,
                sf::Color::Green,
                sf::Color::White,
                24,
                [this]() { startGame(); }
            )
        );

        auto exitButtonEntity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            exitButtonEntity,
            ecs::ButtonFactory::create_button(
                "Exit",
                sf::Vector2f(400.0f, 400.0f),
                sf::Vector2f(200.0f, 50.0f),
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
    registry.emplace_component<ecs::Position>(movable_entity, 400.0f, 300.0f); // Starting position
    registry.emplace_component<ecs::Velocity>(movable_entity, 0.0f, 0.0f);
    registry.emplace_component<ecs::Drawable>(movable_entity, "assets/Ship/Ship.png");
    registry.emplace_component<ecs::Controllable>(movable_entity, true, 5.0f); // Allow control
    registry.emplace_component<ecs::Acceleration>(movable_entity, 0.0f, 0.0f);

    std::cout << "Game started!" << std::endl;
}
}


