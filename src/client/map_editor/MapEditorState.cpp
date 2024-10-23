/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MapEditorState
*/

#include "MapEditorState.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <cmath>

namespace rtype {

    float snapToGrid(float value) {
        return std::round(value / MapEditorState::GRID_SIZE) * MapEditorState::GRID_SIZE;
    }

    MapEditorState::MapEditorState(sf::RenderWindow& window)
        : window(window), currentWave(1), previousWave(1) {
        registry.register_all_components();
        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }
        if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Could not load shader");
        }
        if (!enemyTexture.loadFromFile("assets/sprites/r-type-enemy.gif", sf::IntRect(0, 0, 32, 32))) {
            throw std::runtime_error("Failed to load texture for enemy1.");
        }
        if (!enemy2Texture.loadFromFile("assets/shooting_enemy.png")) {
            throw std::runtime_error("Failed to load texture for enemy2.");
        }
        for (int i = 1; i <= 10; i++) {
            waves.emplace_back(i);
        }
    }

    void MapEditorState::handleInput() {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    auto mousePosWorld = window.mapPixelToCoords(mousePos);

                    float snappedX = snapToGrid(mousePosWorld.x);
                    float snappedY = snapToGrid(mousePosWorld.y);

                    if (currentWave - 1 < static_cast<int>(waves.size())) {
                        waves[currentWave - 1].addMob(snappedX, snappedY, currentMobType);
                        std::cout << "Added mob at: " << snappedX << ", " << snappedY << std::endl;
                        std::cout << "Current wave: " << currentWave << std::endl;
                    } else {
                        std::cerr << "Invalid wave index" << std::endl;
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Right) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    auto mousePosWorld = window.mapPixelToCoords(mousePos);

                    if (currentWave - 1 < static_cast<int>(waves.size())) {
                        auto& mobs = waves[currentWave - 1].mobs;

                        float closestDistanceSq = std::numeric_limits<float>::max();
                        int closestIndex = -1;

                        for (size_t i = 0; i < mobs.size(); ++i) {
                            float dx = mobs[i].x - mousePosWorld.x;
                            float dy = mobs[i].y - mousePosWorld.y;
                            float distanceSq = dx * dx + dy * dy;

                            if (distanceSq < closestDistanceSq && distanceSq < 400.0f) {
                                closestDistanceSq = distanceSq;
                                closestIndex = static_cast<int>(i);
                            }
                        }

                        if (closestIndex != -1) {
                            mobs.erase(mobs.begin() + closestIndex);
                            std::cout << "Removed mob at index: " << closestIndex << std::endl;
                        } else {
                            std::cerr << "No mob found near the click position." << std::endl;
                        }
                    } else {
                        std::cerr << "Invalid wave index" << std::endl;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    if (currentWave > 1) {
                        switchWave(currentWave, currentWave - 1);
                    } else {
                        switchWave(currentWave, 10);
                    }
                }

                if (event.key.code == sf::Keyboard::Right) {
                    if (currentWave < static_cast<int>(waves.size())) {
                        switchWave(currentWave, currentWave + 1);
                    } else {
                        switchWave(currentWave, 1);
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                    saveWavesToJson();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                    switchWave(currentWave, 1);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                    switchWave(currentWave, 2);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
                    switchWave(currentWave, 3);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
                    switchWave(currentWave, 4);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
                    switchWave(currentWave, 5);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
                    switchWave(currentWave, 6);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
                    switchWave(currentWave, 7);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
                    switchWave(currentWave, 8);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) {
                    switchWave(currentWave, 9);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
                    switchWave(currentWave, 10);
                }

                if (event.key.code == sf::Keyboard::M) {
                    if (currentMobType == "enemy1") {
                        currentMobType = "enemy2";
                    } else {
                        currentMobType = "enemy1";
                    }
                    std::cout << "Current mob type: " << currentMobType << std::endl;
                }
            }
        }
    }

    void MapEditorState::drawHotbar() {
        sf::Sprite hotbar;
        float scale = 1.5;
        static sf::Texture hotbarTexture;
        if (!hotbarTexture.loadFromFile("assets/sprites/Hotbar.png")) {
            std::cerr << "Failed to load texture for hotbar." << std::endl;
            return;
        }
        hotbar.setTexture(hotbarTexture);
        hotbar.setScale(scale, scale);
        auto windowSize = window.getSize();
        float hotbarY = windowSize.y - hotbarTexture.getSize().y * scale;
        hotbar.setPosition(windowSize.x / 2 - hotbarTexture.getSize().x * scale / 2, hotbarY);
        window.draw(hotbar);
    }

    void MapEditorState::displayWave() {
        for (const auto& wave : waves) {
            if (currentWave == wave.wave_id) {
                for (const auto& mob : wave.mobs) {
                    sf::Sprite sprite;
                    if (mob.type == "enemy1") {
                        sprite.setTexture(enemyTexture);
                    } else if (mob.type == "enemy2") {
                        sprite.setTexture(enemy2Texture);
                    }
                    sf::Vector2u textureSize = sprite.getTexture()->getSize();
                    sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
                    sprite.setPosition(mob.x, mob.y);
                    window.draw(sprite);
                }
            }
        }
    }

    bool MapEditorState::switchWave(int previousWave, int newWave) {
        std::cout << "Switching from wave " << previousWave << " to wave " << newWave << std::endl;

        if (newWave - 1 < static_cast<int>(waves.size())) {
            currentWave = newWave;
            return true;
        } else {
            return false;
        }
    }

    void MapEditorState::update() {
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system(registry, window, backgroundShader);
        }

        mousePosWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    }

    void MapEditorState::render() {
        window.clear();
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system_render(registry, window, backgroundShader);
        } else {
            sf::Shader::bind(nullptr);
        }

        renderGrid();
        renderPreview();
        displayWave();
        drawHUD();
        window.display();
    }

    void MapEditorState::renderGrid() {
        sf::RectangleShape line;

        line.setFillColor(sf::Color(200, 200, 200, 50));

        for (float x = 0; x <= window.getSize().x; x += GRID_SIZE) {
            line.setPosition(x, 0);
            line.setSize(sf::Vector2f(1, window.getSize().y));
            window.draw(line);
        }

        for (float y = 0; y <= window.getSize().y; y += GRID_SIZE) {
            line.setPosition(0, y);
            line.setSize(sf::Vector2f(window.getSize().x, 1));
            window.draw(line);
        }
    }

    void MapEditorState::renderPreview() {
        sf::Sprite sprite;
        sf::Texture previewTexture;
        if (currentMobType == "enemy1") {
            previewTexture = enemyTexture;
        } else if (currentMobType == "enemy2") {
            previewTexture = enemy2Texture;
        }

        sprite.setTexture(previewTexture);

        sf::Vector2u textureSize = previewTexture.getSize();
        sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);

        sf::Vector2f mousePosPreview = {
            snapToGrid(mousePosWorld.x),
            snapToGrid(mousePosWorld.y)
        };
        sprite.setPosition(mousePosPreview.x, mousePosPreview.y);
        sprite.setColor(sf::Color(128, 128, 128, 128));

        window.draw(sprite);
    }

    void MapEditorState::drawHUD() {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setString("Wave: " + std::to_string(currentWave));
        window.draw(text);

        drawHotbar();
    }

    void MapEditorState::saveWavesToJson() {
        nlohmann::json jsonLevels;

        nlohmann::json jsonLevel;
        jsonLevel["level_id"] = 1;

        for (const auto& wave : waves) {
            nlohmann::json jsonWave;
            jsonWave["wave_id"] = wave.wave_id;

            for (const auto& mob : wave.mobs) {
                nlohmann::json jsonMob;
                jsonMob["x"] = mob.x;
                jsonMob["y"] = mob.y;
                jsonMob["type"] = mob.type;

                jsonWave["mobs"].push_back(jsonMob);
            }

            jsonLevel["waves"].push_back(jsonWave);
        }

        jsonLevels["levels"].push_back(jsonLevel);

        std::ofstream file("waves.json");
        if (file.is_open()) {
            file << jsonLevels.dump(4);
            file.close();
            std::cout << "Waves saved to 'waves.json'" << std::endl;
        } else {
            std::cerr << "Unable to save the waves." << std::endl;
        }
    }
}
