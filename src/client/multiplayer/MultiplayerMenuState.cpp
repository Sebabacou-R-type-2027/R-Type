#include "MultiplayerMenuState.hpp"
#include <iostream>

namespace rtype {
    MultiplayerMenuState::MultiplayerMenuState(sf::RenderWindow& window)
        : window(window), port(std::nullopt), activeField(NONE) {
        registry.register_all_components();

        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }

        if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Could not load shader");
        }
        initUI();
    }

    void MultiplayerMenuState::initUI() {
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
                [this]() {
                    std::string hostAddress = "";
                    std::string port = "";
                    std::cout << "Connecting to server at " << hostAddress << ":" << port << std::endl;

                }
            )
        );
    }

void MultiplayerMenuState::handleInput() {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        if (event.type == sf::Event::TextEntered) {
            if (activeField == ADDRESS) {
                if (event.text.unicode < 128) {
                    hostAddress += static_cast<char>(event.text.unicode);
                    addressText.setString(hostAddress);
                } else if (event.text.unicode == '\b' && !hostAddress.empty()) {
                    hostAddress.pop_back();
                    addressText.setString(hostAddress);
                }
            } else if (activeField == PORT) {
                if (isdigit(event.text.unicode)) {
                    portInput += static_cast<char>(event.text.unicode);
                    portText.setString(portInput);
                } else if (event.text.unicode == '\b' && !portInput.empty()) {
                    portInput.pop_back();
                    portText.setString(portInput);
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (inputRectAddressPos.x <= mousePos.x && mousePos.x <= inputRectAddressPos.x + 400.0f &&
                inputRectAddressPos.y <= mousePos.y && mousePos.y <= inputRectAddressPos.y + 40.0f) {
                activeField = ADDRESS;
            } else if (inputRectPortPos.x <= mousePos.x && mousePos.x <= inputRectPortPos.x + 400.0f &&
                       inputRectPortPos.y <= mousePos.y && mousePos.y <= inputRectPortPos.y + 40.0f) {
                activeField = PORT;
            }
        }
    }
}



    void MultiplayerMenuState::update() {
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system(registry, window, backgroundShader);
        }

        inputRectAddressPos = { window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - 400.0f + 140.0f };
        inputRectPortPos = { window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - 400.0f + 250.0f };
        HitboxAddress.setSize(sf::Vector2f(400.0f, 40.0f));
        HitboxAddress.setPosition(inputRectAddressPos);
        HitboxPort.setSize(sf::Vector2f(400.0f, 40.0f));
        HitboxPort.setPosition(inputRectPortPos);
        HitboxAddress.setFillColor(sf::Color::Transparent);
        HitboxPort.setFillColor(sf::Color::Transparent);
        HitboxAddress.setOutlineThickness(1);
        HitboxPort.setOutlineThickness(1);
        HitboxAddress.setOutlineColor(sf::Color::White);
        HitboxPort.setOutlineColor(sf::Color::White);

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
        window.draw(HitboxAddress);
        window.draw(HitboxPort);

        window.display();
    }
}
