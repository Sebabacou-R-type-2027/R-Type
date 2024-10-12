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

    sf::Shader::bind(&shader); // Ensure the shader is bound
    try {
        shader.setUniform("iTime", static_cast<float>(clock.getElapsedTime().asSeconds())); // Pass time
        shader.setUniform("iResolution", sf::Glsl::Vec3(window.getSize().x, window.getSize().y, 1.0f)); // Pass resolution
    } catch (const std::exception& e) {
        std::cout << "Error setting shader uniforms: " << e.what() << std::endl;
        std::cerr << "Error setting shader uniforms: " << e.what() << std::endl;
    }
    // shader.setUniform("iTime", static_cast<float>(clock.getElapsedTime().asSeconds())); // Pass time
    // shader.setUniform("iResolution", sf::Glsl::Vec3(window.getSize().x, window.getSize().y, 1.0f)); // Pass resolution
}




void ShaderSystem::render(Registry& registry, sf::RenderWindow& window, sf::Shader& shader) {
    sf::RectangleShape fullscreenRect(sf::Vector2f(window.getSize().x, window.getSize().y));
    fullscreenRect.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(fullscreenRect, &shader);
}

}
