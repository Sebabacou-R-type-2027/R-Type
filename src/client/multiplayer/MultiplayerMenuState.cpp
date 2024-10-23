#include "MultiplayerMenuState.hpp"
#include "utils/Settings.hpp"
#include <iostream>
#include <cctype>

namespace rtype {
    MultiplayerMenuState::MultiplayerMenuState(sf::RenderWindow& window, client::Client& network, Game& game)
        : window(window), port(std::nullopt), activeField(ADDRESS), game(game), network_(network) {
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

        float width = 450.0f * 2;
        float height = 400.0f * 2;

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

        // Username Text
        auto usernameTextEntity = registry.spawn_entity();
        auto& usernameDrawable = registry.emplace_component<ecs::Drawable>(usernameTextEntity, "assets/fonts/arial.ttf", "Username :", 24, sf::Color(255, 255, 255, 255 * 0.9));
        registry.emplace_component<ecs::Position>(usernameTextEntity, window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 320.0f);

        // Input rectangle for Username
        auto inputRectUsernameEntity = registry.spawn_entity();
        auto& inputRectUsernameDrawable = registry.emplace_component<ecs::Drawable>(inputRectUsernameEntity, 400.0f, 40.0f, sf::Color(30, 30, 30, 255 * 0.8), sf::Color::Black);
        registry.emplace_component<ecs::Position>(inputRectUsernameEntity, window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 360.0f);

        // Password Text
        auto passwordTextEntity = registry.spawn_entity();
        auto& passwordDrawable = registry.emplace_component<ecs::Drawable>(passwordTextEntity, "assets/fonts/arial.ttf", "Password :", 24, sf::Color(255, 255, 255, 255 * 0.9));
        registry.emplace_component<ecs::Position>(passwordTextEntity, window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 430.0f);

        // Input rectangle for Password
        auto inputRectPasswordEntity = registry.spawn_entity();
        auto& inputRectPasswordDrawable = registry.emplace_component<ecs::Drawable>(inputRectPasswordEntity, 400.0f, 40.0f, sf::Color(30, 30, 30, 255 * 0.8), sf::Color::Black);
        registry.emplace_component<ecs::Position>(inputRectPasswordEntity, window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 470.0f);

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
                    Settings::getInstance().username = username;
                    Settings::getInstance().password = password;
                    std::cout << "Connecting to server at " << Settings::getInstance().serverAddress << ":" << Settings::getInstance().serverPort << std::endl;
                    std::cout << "Username: " << Settings::getInstance().username << std::endl;
                    std::cout << "Password: " << Settings::getInstance().password << std::endl;
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

        usernameText.setFont(font);
        usernameText.setCharacterSize(24);
        usernameText.setFillColor(sf::Color::White);
        usernameText.setPosition(window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 360.0f + 5.0f);

        passwordText.setFont(font);
        passwordText.setCharacterSize(24);
        passwordText.setFillColor(sf::Color::White);
        passwordText.setPosition(window.getSize().x / 2.0f - 200.0f, window.getSize().y / 2.0f - height / 2.0f + 470.0f + 5.0f);
    }


    void MultiplayerMenuState::handleInput() {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                game.changeState(std::make_shared<MainMenuState>(window, game, network_));
                break;
            }

            if (event.type == sf::Event::TextEntered) {
                char enteredChar = static_cast<char>(event.text.unicode);
                if (activeField == ADDRESS) {
                    printf("Entered char address: %c\n", enteredChar);
                    if ((isdigit(enteredChar) || enteredChar == '.') && hostAddress.size() < 15) {
                        hostAddress += enteredChar;
                        addressText.setString(hostAddress);
                    } else if (event.text.unicode == '\b' && !hostAddress.empty()) {
                        hostAddress.pop_back();
                        addressText.setString(hostAddress);
                    }
                } else if (activeField == PORT) {
                    printf("Entered char port: %c\n", enteredChar);
                    if (isdigit(enteredChar) && portInput.size() < 5) {
                        portInput += enteredChar;
                        portText.setString(portInput);
                    } else if (event.text.unicode == '\b' && !portInput.empty()) {
                        portInput.pop_back();
                        portText.setString(portInput);
                    }
                } else if (activeField == USERNAME) {
                    printf("Entered char username: %c\n", enteredChar);
                    if (isalnum(enteredChar) && username.size() < 15) {
                        username += enteredChar;
                        usernameText.setString(username);
                    } else if (event.text.unicode == '\b' && !username.empty()) {
                        username.pop_back();
                        usernameText.setString(username);
                    }
                } else if (activeField == PASSWORD) {
                    printf("Entered char password: %c\n", enteredChar);
                    if (isalnum(enteredChar) && password.size() < 15) {
                        password += enteredChar;
                        secretpassword += '*';
                        passwordText.setString(secretpassword);
                    } else if (event.text.unicode == '\b' && !password.empty()) {
                        password.pop_back();
                        secretpassword.pop_back();
                        passwordText.setString(secretpassword);
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    activeField = static_cast<InputField>((activeField + 1) % 4);
                    printf("Active field: %d\n", activeField);
                }
                // Handle delete key
                if (event.key.code == sf::Keyboard::Delete) {
                    if (activeField == ADDRESS && !hostAddress.empty()) {
                        hostAddress.pop_back();
                        addressText.setString(hostAddress);
                    } else if (activeField == PORT && !portInput.empty()) {
                        portInput.pop_back();
                        portText.setString(portInput);
                    } else if (activeField == USERNAME && !username.empty()) {
                        username.pop_back();
                        usernameText.setString(username);
                    } else if (activeField == PASSWORD && !password.empty()) {
                        password.pop_back();
                        secretpassword.pop_back();
                        if (showPassword) {
                            passwordText.setString(password);
                        } else {
                            passwordText.setString(secretpassword);
                        }
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
        window.draw(usernameText);
        window.draw(passwordText);
        window.display();
    }
}
