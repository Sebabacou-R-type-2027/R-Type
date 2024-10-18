#include "MapEditorState.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <cmath> // For std::sqrt and std::pow


namespace rtype {

    // Helper function to snap a value to the nearest grid point
    float snapToGrid(float value) {
        return std::round(value / MapEditorState::GRID_SIZE) * MapEditorState::GRID_SIZE;
    }

    MapEditorState::MapEditorState(sf::RenderWindow& window)
        : window(window), currentWave(1) {
        registry.register_all_components();
        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }
        if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Could not load shader");
        }
        for (int i = 1; i <= 10; i++) {
            waves.emplace_back(i); // Using emplace_back for efficiency
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

                    // Snap to grid
                    float snappedX = snapToGrid(mousePosWorld.x);
                    float snappedY = snapToGrid(mousePosWorld.y);

                    // Ensure currentWave is valid
                    if (currentWave - 1 < static_cast<int>(waves.size())) {
                        waves[currentWave - 1].addMob(snappedX, snappedY, "enemy1");
                        std::cout << "Added mob at: " << snappedX << ", " << snappedY << std::endl;
                        std::cout << "Current wave: " << currentWave << std::endl;
                    } else {
                        std::cerr << "Invalid wave index" << std::endl;
                    }
                }

                // Remove mob on right click
                if (event.mouseButton.button == sf::Mouse::Right) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    auto mousePosWorld = window.mapPixelToCoords(mousePos);

                    if (currentWave - 1 < static_cast<int>(waves.size())) {
                        auto& mobs = waves[currentWave - 1].mobs;

                        float closestDistanceSq = std::numeric_limits<float>::max();
                        int closestIndex = -1;

                        // Iterate over mobs to find the closest one
                        for (size_t i = 0; i < mobs.size(); ++i) {
                            float dx = mobs[i].x - mousePosWorld.x;
                            float dy = mobs[i].y - mousePosWorld.y;
                            float distanceSq = dx * dx + dy * dy;

                            // Find the closest mob within a certain range (e.g., 20 units)
                            if (distanceSq < closestDistanceSq && distanceSq < 400.0f) { // 20^2 = 400
                                closestDistanceSq = distanceSq;
                                closestIndex = static_cast<int>(i);
                            }
                        }

                        // If a mob is close enough, remove it
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
                if (event.key.code == sf::Keyboard::Space) {
                    if (switchWave(currentWave, currentWave + 1)) {
                        std::cout << "Switching to wave: " << currentWave + 1 << std::endl;
                    } else {
                        switchWave(currentWave, 1);
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                    saveWavesToJson();

                // for (int key = sf::Keyboard::Num1; key <= sf::Keyboard::Num0; ++key) {
                //     if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key))) {
                //         int waveNumber = (key == sf::Keyboard::Num0) ? 10 : (key - sf::Keyboard::Num1 + 1);
                //         switchWave(currentWave, waveNumber);
                //         break; // Prevent multiple switches in one frame
                //     }
                // }
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
            }
        }
    }

    void MapEditorState::displayWave() {
        for (const auto& wave : waves) {
            if (currentWave == wave.wave_id) {
                for (const auto& mob : wave.mobs) {
                    if (mob.type == "enemy1") {
                        sf::Sprite sprite;
                        static sf::Texture texture; // Make texture static to avoid reloading every frame
                        if (!texture.loadFromFile("assets/sprites/r-type-enemy.gif", sf::IntRect(0, 0, 32, 32))) {
                            std::cerr << "Failed to load texture for enemy1." << std::endl;
                            continue;
                        }
                        sf::Vector2u textureSize = texture.getSize();
                        sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
                        sprite.setTexture(texture);
                        sprite.setPosition(mob.x, mob.y);
                        window.draw(sprite);
                    }
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
    }

    void MapEditorState::render() {
        window.clear();
        if (Settings::getInstance().isShaderEnabled) {
            system.shader_system_render(registry, window, backgroundShader);
        } else {
            sf::Shader::bind(nullptr);
        }

        // Optional: Render grid
        renderGrid();

        displayWave();
        window.display();
    }

    void MapEditorState::renderGrid() {
        sf::RectangleShape line;
        line.setFillColor(sf::Color(200, 200, 200, 50)); // Light gray with some transparency

        // Vertical lines
        for (float x = 0; x <= window.getSize().x; x += GRID_SIZE) {
            line.setPosition(x, 0);
            line.setSize(sf::Vector2f(1, window.getSize().y));
            window.draw(line);
        }

        // Horizontal lines
        for (float y = 0; y <= window.getSize().y; y += GRID_SIZE) {
            line.setPosition(0, y);
            line.setSize(sf::Vector2f(window.getSize().x, 1));
            window.draw(line);
        }
    }

    void MapEditorState::saveWavesToJson() {
        nlohmann::json jsonWaves;

        jsonWaves["level"] = 1;

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

            jsonWaves["waves"].push_back(jsonWave);
        }

        std::ofstream file("waves.json");
        if (file.is_open()) {
            file << jsonWaves.dump(4);
            file.close();
            std::cout << "Waves saved to 'waves.json'" << std::endl;
        } else {
            std::cerr << "Unable to save the waves." << std::endl;
        }
    }
}
