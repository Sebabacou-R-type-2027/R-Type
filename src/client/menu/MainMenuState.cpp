#include "MainMenuState.hpp"
#include "../game/Game.hpp"
#include "../factories/button_factory.hpp"
#include <iostream>
#include "../game/GamePlayState.hpp"

namespace rtype {
    MainMenuState::MainMenuState(sf::RenderWindow& window, Game& game, client::Client& network)
        : window(window), game(game), registry(game.getRegistry()), system(game.getSystem()), network_(network),  backgroundShader(game.getBackgroundShader())  // Initialize registry and system here
    {
        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }

        registry.register_all_components();

        createMenuButtons();
        createMenuTitle();
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
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system(registry, window, backgroundShader);
        }

        system.button_system(registry, window);
        // fpsCounter.update();
    }

    void MainMenuState::render() {
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

    void MainMenuState::startGame() {
        std::cout << "Starting the game..." << std::endl;
        sf::Shader::bind(nullptr);
        // delete the FPS counter
        game.changeState(std::make_shared<GamePlayState>(window, network_, true)); // TODO : change to false when multiplayer is implemented
    }

    void MainMenuState::startMultiplayer() {
        std::cout << "Starting multiplayer..." << std::endl;
        sf::Shader::bind(nullptr);
        game.changeState(std::make_shared<MultiplayerMenuState>(window, network_));
    }

    void MainMenuState::disableShader() {
        Settings::getInstance().isShaderEnabled = false;
        sf::Shader::bind(nullptr);
    }

    void MainMenuState::enableShader() {
        sf::Shader::bind(&backgroundShader);
        Settings::getInstance().isShaderEnabled = true;
    }

    void MainMenuState::createMenuTitle() {
        auto title = registry.spawn_entity();
        auto& drawable = registry.emplace_component<ecs::Drawable>(title, "assets/fonts/arial.ttf", "R-Type", 96, sf::Color::Blue);
        sf::FloatRect textBounds = drawable->text.getGlobalBounds();
        float centerX = (window.getSize().x - textBounds.width) / 2.0f;
        registry.emplace_component<ecs::Position>(title, centerX, 50.0f);
    }

    void MainMenuState::createMenuButtons() {
        sf::Vector2u window_size = window.getSize();
        float button_height = 50.0f;
        float spacing = 20.0f;
        float total_height = (button_height + spacing) * 7;
        float start_y = (window_size.y - total_height) / 2.0f;

        auto createButton = [this, button_height](const std::string& label, float yPos, std::function<void()> callback) -> ecs::Entity {
            sf::Text text(label, font, 24);
            float button_width = text.getLocalBounds().width + 40.0f; // Add padding to the text width
            float button_x = (window.getSize().x - button_width) / 2.0f; // Center the button

            button_x += button_width / 2.0f;

            auto entity = registry.spawn_entity();
            registry.emplace_component<ecs::Button>(
                entity,
                ecs::ButtonFactory::create_button(
                    label,
                    sf::Vector2f(button_x, yPos),
                    sf::Vector2f(button_width, button_height),
                    font,
                    sf::Color(14, 94, 255, 255),
                    sf::Color(7, 115, 255, 255),
                    sf::Color::Blue,
                    sf::Color::White,
                    24,
                    callback
                )
            );
            return entity;
        };

        float yPos = start_y;
        createButton("Solo Campaign", yPos, [this]() { startGame(); });
        yPos += button_height + spacing;

        createButton("Multiplayer", yPos, [this]() { startMultiplayer(); });
        yPos += button_height + spacing;

        createButton("Customize Ship", yPos, [this]() { startGame(); });
        yPos += button_height + spacing;

        createButton("Settings", yPos, [this]() { startGame(); });
        yPos += button_height + spacing;

        createButton("Map Editor", yPos, [this]() { startGame(); });
        yPos += button_height + spacing;

        createButton("Game Options (ON/OFF Shader)", yPos, [this]() { Settings::getInstance().isShaderEnabled ? disableShader() : enableShader(); });
        yPos += button_height + spacing;

        createButton("Quit", yPos, [this]() { window.close(); });
    }
}