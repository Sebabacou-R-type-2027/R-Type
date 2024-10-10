#include "MainMenuState.hpp"
#include "../game/Game.hpp"
#include "../factories/button_factory.hpp"
#include <iostream>
#include "../game/GamePlayState.hpp"

namespace rtype {
    MainMenuState::MainMenuState(sf::RenderWindow& window, Game& game)
        : window(window), game(game), registry(game.getRegistry()), system(game.getSystem())  // Initialize registry and system here
    {
        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }

        registry.register_all_components();

        createMenuButtons();
    }

    void MainMenuState::handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }
    }

    void MainMenuState::update() {
        system.button_system(registry, window);
    }

    void MainMenuState::render() {
        window.clear();

        system.draw_system(registry, window);
        system.button_system_render(registry, window);

        window.display();
    }

    void MainMenuState::startGame() {
        std::cout << "Starting the game..." << std::endl;
        game.changeState(std::make_unique<GamePlayState>(window));
    }

    void MainMenuState::createMenuButtons() {
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

}
