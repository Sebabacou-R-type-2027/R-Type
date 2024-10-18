#include "MultiplayerMenuState.hpp"
#include <iostream>
#include <cctype>

namespace rtype {
    MultiplayerMenuState::MultiplayerMenuState(sf::RenderWindow& window, client::Client& network)
        : window(window), port(std::nullopt), activeField(ADDRESS) {
        registry.register_all_components();

        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }

        if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Could not load shader");
        }
        initUI(network);
    }

    void MultiplayerMenuState::initUI(client::Client& network) {
        auto multiplayerBackground = registry.spawn_entity();

        float width = 450.0f;
        float height = 400.0f;

        sf::Color fillColor = sf::Color(100, 100, 100, 255 * 0.7);
        sf::Color outlineColor = sf::Color::Blue;
        registry.emplace_component<ecs::Drawable>(multiplayerBackground, width, height, fillColor, outlineColor);
        registry.emplace_component<ecs::Position>(multiplayerBackground, window.getSize().x / 2.0f - width / 2.0f, window.getSize().y / 2.0f - height / 2.0f);

        // Title
        auto titleEntity = registry.spawn_entity();
        auto& titleDrawable = registry.emplace_component<ecs::Drawable>(titleEntity, "assets/fonts/arial.ttf", "Multiplayer", 48, sf::Color::White);
        sf::FloatRect titleBounds = titleDrawable.value().text.getGlobalBounds();
        registry.emplace_component<ecs::Position>(titleEntity, window.getSize().x / 2.0f - titleBounds.width / 2.0f, window.getSize().y / 2.0f - height / 2.0f + 10.0f);

        // Server Address Text
        auto serverAddressTextEntity = registry.spawn_entity();
        auto& serverAddressDrawable = registry.emplace_component<ecs::Drawable>(serverAddressTextEntity, "assets/fonts/arial.ttf", "Server Address :", 24, sf::Color(255, 255, 255, 255 * 0.9));
        registry.emplace_component<ecs::Position>(serverAddressTextEntity, window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 100.0f);

        // Input rectangle for Server Address
        auto inputRectAddressEntity = registry.spawn_entity();
        auto& inputRectAddressDrawable = registry.emplace_component<ecs::Drawable>(inputRectAddressEntity, 400.0f, 40.0f, sf::Color(30, 30, 30, 255 * 0.8), sf::Color::Black);
        registry.emplace_component<ecs::Position>(inputRectAddressEntity, window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 140.0f);

        // Port Text
        auto portTextEntity = registry.spawn_entity();
        auto& portDrawable = registry.emplace_component<ecs::Drawable>(portTextEntity, "assets/fonts/arial.ttf", "Port :", 24, sf::Color(255, 255, 255, 255 * 0.9));
        registry.emplace_component<ecs::Position>(portTextEntity, window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 210.0f);

        // Input rectangle for Port
        auto inputRectPortEntity = registry.spawn_entity();
        auto& inputRectPortDrawable = registry.emplace_component<ecs::Drawable>(inputRectPortEntity, 400.0f, 40.0f, sf::Color(30, 30, 30, 255 * 0.8), sf::Color::Black);
        registry.emplace_component<ecs::Position>(inputRectPortEntity, window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 250.0f);

        // Connect Button
        float buttonWidth = 400.0f;
        float buttonHeight = 50.0f;
        float buttonX = window.getSize().x / 2.0f - buttonWidth / 2.0f;
        float yPos = window.getSize().y / 2.0f + height / 2.0f - buttonHeight - 20.0f;

        buttonX += buttonWidth / 2.0f;

        auto connectButtonEntity = registry.spawn_entity();
        registry.emplace_component<ecs::Button>(
            connectButtonEntity,
            ecs::ButtonFactory::create_button(
                "Connect",
                sf::Vector2f(buttonX, yPos),
                sf::Vector2f(buttonWidth, buttonHeight),
                font,
                sf::Color(14, 94, 255, 255),
                sf::Color(7, 115, 255, 255),
                sf::Color::Blue,
                sf::Color::White,
                24,
                [this, &network]() {
                    Settings::getInstance().serverAddress = hostAddress;
                    Settings::getInstance().serverPort = std::stoi(portInput);
                    std::cout << "Connecting to server at " << Settings::getInstance().serverAddress << ":" << Settings::getInstance().serverPort << std::endl;
                    network.connect(Settings::getInstance().serverAddress, Settings::getInstance().serverPort);
                }
            )
        );

        addressText.setFont(font);
        addressText.setCharacterSize(24);
        addressText.setFillColor(sf::Color::White);
        addressText.setPosition(window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 140.0f + 5.0f);

        portText.setFont(font);
        portText.setCharacterSize(24);
        portText.setFillColor(sf::Color::White);
        portText.setPosition(window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 250.0f + 5.0f);
    }


    void MultiplayerMenuState::handleInput() {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (activeField == ADDRESS) {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    if ((isdigit(enteredChar) || enteredChar == '.') && hostAddress.size() < 15) {
                        hostAddress += enteredChar;
                        addressText.setString(hostAddress);
                    } else if (event.text.unicode == '\b' && !hostAddress.empty()) {
                        hostAddress.pop_back();
                        addressText.setString(hostAddress);
                    }
                } else if (activeField == PORT) {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    if (isdigit(enteredChar) && portInput.size() < 5) {
                        portInput += enteredChar;
                        portText.setString(portInput);
                    } else if (event.text.unicode == '\b' && !portInput.empty()) {
                        portInput.pop_back();
                        portText.setString(portInput);
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    activeField = (activeField == ADDRESS) ? PORT : ADDRESS; // Switch between ADDRESS and PORT
                }
                // Handle delete key
                if (event.key.code == sf::Keyboard::Delete) {
                    if (activeField == ADDRESS && !hostAddress.empty()) {
                        hostAddress.pop_back();
                        addressText.setString(hostAddress);
                    } else if (activeField == PORT && !portInput.empty()) {
                        portInput.pop_back();
                        portText.setString(portInput);
                    }
                }
            }
        }
    }





    void MultiplayerMenuState::update() {
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system(registry, window, backgroundShader);
        }

        system.button_system(registry, window);
    }


    void MultiplayerMenuState::render() {
        window.clear();
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system_render(registry, window, backgroundShader);
        } else {
            sf::Shader::bind(nullptr);
        }

        system.draw_system(registry, window);
        system.button_system_render(registry, window);

        // Render the text for input fields
        window.draw(addressText);
        window.draw(portText);

        window.display();
    }
}
