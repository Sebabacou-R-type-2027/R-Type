/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** button_system
*/

#include "shader_system.hpp"
#include <time.h>
#include <iostream>

namespace ecs::systems {

void ShaderSystem::update(Registry& registry, sf::RenderWindow& window, sf::Shader& shader) {
    if (!shader.isAvailable()) {
        std::cerr << "Shader is not available!" << std::endl;
        return; // Early exit if shader is not valid
    }

    sf::Shader::bind(&shader); // Bind the shader first
    try {
        shader.setUniform("iTime", static_cast<float>(clock.getElapsedTime().asSeconds())); // Pass time
        shader.setUniform("iResolution", sf::Glsl::Vec3(window.getSize().x, window.getSize().y, 1.0f)); // Pass resolution
    } catch (const std::exception& e) {
        std::cerr << "Error setting shader uniforms: " << e.what() << std::endl;
    }
}

void ShaderSystem::render(Registry& registry, sf::RenderWindow& window, sf::Shader& shader) {
    sf::View currentView = window.getView();
    sf::Vector2f viewCenter = currentView.getCenter();
    sf::Vector2f viewSize = currentView.getSize();

    sf::RectangleShape fullscreenRect(viewSize);
    fullscreenRect.setPosition(viewCenter.x - viewSize.x / 2, viewCenter.y - viewSize.y / 2);
    fullscreenRect.setFillColor(sf::Color(0, 0, 0, 100));

    window.draw(fullscreenRect, &shader);

    sf::Shader::bind(nullptr);
}


}
