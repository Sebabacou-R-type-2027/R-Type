/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** LobbyState
*/

#include "LobbyState.hpp"
#include "../menu/MainMenuState.hpp"
#include "Client.hpp"
#include "Position.hpp"
#include "registry.hpp"
#include "system.hpp"
#include "utils/Settings.hpp"

namespace rtype {
    LobbyState::LobbyState(sf::RenderWindow& window, client::Client& network, Game& game)
        : _window(window), game(game), network_(network), registry(game.getRegistry()), system(game.getSystem()) {

        registry.register_all_components();

        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }

        if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Could not load shader");
        }
        initUI(network);
    }

    void LobbyState::initUI(client::Client &network) {
        float width = 550.0f;
        float height = 400.0f;
        auto LobbyBackground = registry.spawn_entity();

        sf::Color fillColor = sf::Color(100, 100, 100, 255 * 0.7);
        sf::Color outlineColor = sf::Color::Blue;
        registry.emplace_component<ecs::Drawable>(LobbyBackground, width, height, fillColor, outlineColor);
        registry.emplace_component<ecs::Position>(LobbyBackground, _window.getSize().x / 2.0f - width / 2.0f, _window.getSize().y / 2.0f - height / 2.0f);

        float InputFieldHeight = 50.0f;
        float InputFieldWidth = 200.0f;

        // CREATE LOBBY
        auto CreateLobbyEntity = registry.spawn_entity();
        auto& CreateLobbyDrawable = registry.emplace_component<ecs::Drawable>(CreateLobbyEntity, "assets/fonts/arial.ttf", "Create lobby:", 24, sf::Color::White);
        sf::FloatRect CreateLobbyBounds = CreateLobbyDrawable.value().text.getGlobalBounds();
        registry.emplace_component<ecs::Position>(CreateLobbyEntity, _window.getSize().x / 2.0f - CreateLobbyBounds.width / 2.0f - 150.0f, _window.getSize().y / 2.0f - height / 2.0f + 10.0f);

        // CREATE LOBBY INPUT
        auto inputRectCreateLobbyEntity = registry.spawn_entity();
        auto &inputRectCreateLobbyDrawable = registry.emplace_component<ecs::Drawable>(inputRectCreateLobbyEntity, InputFieldWidth, InputFieldHeight, sf::Color(30, 30, 30, 255 * 0.8), sf::Color::Black);
        registry.emplace_component<ecs::Position>(inputRectCreateLobbyEntity, _window.getSize().x / 2.0f - InputFieldWidth / 2.0f - 140.0f, _window.getSize().y / 2.0f - height / 2.0f + 50.0f);

        inputRectCreateLobbyDrawablePtr = &inputRectCreateLobbyDrawable.value();

        // JOIN LOBBY
        auto JoinLobbyEntity = registry.spawn_entity();
        auto& JoinLobbyDrawable = registry.emplace_component<ecs::Drawable>(JoinLobbyEntity, "assets/fonts/arial.ttf", "Join lobby:", 24, sf::Color::White);
        sf::FloatRect JoinLobbyBounds = JoinLobbyDrawable.value().text.getGlobalBounds();
        registry.emplace_component<ecs::Position>(JoinLobbyEntity, _window.getSize().x / 2.0f + JoinLobbyBounds.width / 2.0f + 30.0f, _window.getSize().y / 2.0f - height / 2.0f + 10.0f);


        // JOIN LOBBY INPUT
        auto inputRectJoinLobbyEntity = registry.spawn_entity();
        auto &inputRectJoinLobbyDrawable = registry.emplace_component<ecs::Drawable>(inputRectJoinLobbyEntity, InputFieldWidth, InputFieldHeight, sf::Color(30, 30, 30, 255 * 0.8), sf::Color::Black);
        // they need to be symmetric by the center of the screen
        registry.emplace_component<ecs::Position>(inputRectJoinLobbyEntity, _window.getSize().x / 2.0f - InputFieldWidth / 2.0f + 140.0f, _window.getSize().y / 2.0f - height / 2.0f + 50.0f);


        inputRectJoinLobbyDrawablePtr = &inputRectJoinLobbyDrawable.value();

        auto createButton = [this](const std::string& label, float xPos, float yPos, std::function<void()> callback) -> ecs::Entity {
            sf::Text text(label, font, 24);
            float button_width = text.getLocalBounds().width + 40.0f;
            float button_height = 30.0f;

            auto entity = registry.spawn_entity();
            registry.emplace_component<ecs::Button>(
                entity,
                ecs::ButtonFactory::create_button(
                    label,
                    sf::Vector2f(xPos, yPos),
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

        // Use the createButton function to create buttons
        createButton(
            "Create lobby",
            _window.getSize().x / 2.0f - 145.0f,
            _window.getSize().y / 2.0f - height / 2.0f + 120.0f,
            [this, &network]() {
                Settings::getInstance().CreateLobbyID = CreateLobbyInput;
                std::cout << "CreateLobbyID " << Settings::getInstance().CreateLobbyID << std::endl;
            }
        );

        createButton(
            "Join lobby",
            _window.getSize().x / 2.0f + 145.0f,
            _window.getSize().y / 2.0f - height / 2.0f + 120.0f,
            [this, &network]() {
                Settings::getInstance().JoinLobbyID = CreateLobbyInput;
                std::cout << "JoinLobbyID " << Settings::getInstance().JoinLobbyID << std::endl;
            }
        );

        createButton(
            "Join matchmaking",
            _window.getSize().x / 2.0f,
            _window.getSize().y / 2.0f - height / 2.0f + 370.0f,
            []() { std::cout << "Join matchmaking" << std::endl; }
        );


        CreateLobbyText.setFont(font);
        CreateLobbyText.setCharacterSize(24);
        CreateLobbyText.setFillColor(sf::Color::White);
        CreateLobbyText.setPosition(_window.getSize().x / 2.0f - CreateLobbyBounds.width / 2.0f - 160.0f, _window.getSize().y / 2.0f - height / 2.0f + 50.0f + 5.0f);

        JoinLobbyText.setFont(font);
        JoinLobbyText.setCharacterSize(24);
        JoinLobbyText.setFillColor(sf::Color::White);
        JoinLobbyText.setPosition(_window.getSize().x / 2.0f + JoinLobbyBounds.width / 2.0f - 20.0f, _window.getSize().y / 2.0f - height / 2.0f + 50.0f + 5.0f);

        inputRectCreateLobbyDrawablePtr->rectangle.setOutlineColor(sf::Color::White);

    }

    void LobbyState::handleInput() {
        sf::Event event;
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                game.changeState(std::make_shared<MainMenuState>(_window, game, network_));
                break;
            }
            if (event.type == sf::Event::TextEntered) {
                char enteredChar = static_cast<char>(event.text.unicode);
                if (activeField == CREATE) {
                    printf("Entered char address: %c\n", enteredChar);
                    if ((isdigit(enteredChar) || enteredChar == '.') && CreateLobbyInput.size() < 15) {
                        CreateLobbyInput += enteredChar;
                        CreateLobbyText.setString(CreateLobbyInput);
                    } else if (event.text.unicode == '\b' && !CreateLobbyInput.empty()) {
                        CreateLobbyInput.pop_back();
                        CreateLobbyText.setString(CreateLobbyInput);
                    }
                }
                if (activeField == JOIN) {
                    printf("Entered char address: %c\n", enteredChar);
                    if ((isdigit(enteredChar) || enteredChar == '.') && JoinLobbyInput.size() < 15) {
                        JoinLobbyInput += enteredChar;
                        JoinLobbyText.setString(JoinLobbyInput);
                    } else if (event.text.unicode == '\b' && !JoinLobbyInput.empty()) {
                        JoinLobbyInput.pop_back();
                        JoinLobbyText.setString(JoinLobbyInput);
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    activeField = static_cast<InputField>((activeField + 1) % 2);
                    printf("Active field: %d\n", activeField);
                    if (activeField == CREATE) {
                        inputRectCreateLobbyDrawablePtr->rectangle.setOutlineColor(sf::Color::White);
                        inputRectJoinLobbyDrawablePtr->rectangle.setOutlineColor(sf::Color::Black);
                    } else {
                        inputRectCreateLobbyDrawablePtr->rectangle.setOutlineColor(sf::Color::Black);
                        inputRectJoinLobbyDrawablePtr->rectangle.setOutlineColor(sf::Color::White);
                    }
                }
                if (event.key.code == sf::Keyboard::Delete) {
                    if (activeField == CREATE && !CreateLobbyInput.empty()) {
                        CreateLobbyInput.pop_back();
                        CreateLobbyText.setString(CreateLobbyInput);
                    }
                    if (activeField == JOIN && !JoinLobbyInput.empty()) {
                        JoinLobbyInput.pop_back();
                        JoinLobbyText.setString(JoinLobbyInput);
                    }
                }
            }
        }
    }

    void LobbyState::update()
    {
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system(registry, _window, backgroundShader);
        }

        system.button_system(registry, _window);
    }

    void LobbyState::render() {
        _window.clear();
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system_render(registry, _window, backgroundShader);
        } else {
            sf::Shader::bind(nullptr);
        }

        system.draw_system(registry, _window);
        system.button_system_render(registry, _window);

        _window.draw(CreateLobbyText);
        _window.draw(JoinLobbyText);

        _window.display();
    }

}
