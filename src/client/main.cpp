#include <SFML/Graphics.hpp>
#include <iostream>

#include "registry.hpp"
#include "components.hpp"
#include "systems.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Game");
    window.setFramerateLimit(60);

    Registry reg;
    System system;

    reg.register_all_components();

    auto movable_entity = reg.spawn_entity();
    reg.emplace_component<Position>(movable_entity, 400.0f, 300.0f);
    reg.emplace_component<Velocity>(movable_entity, 0.0f, 0.0f);
    reg.emplace_component<Drawable>(movable_entity, true, sf::Sprite(), sf::Texture(), sf::IntRect(), sf::Color::Red, 50.0f, "square");
    reg.emplace_component<Controllable>(movable_entity, true, 5.0f);
    reg.emplace_component<Acceleration>(movable_entity, 0.0f, 0.0f);
    reg.emplace_component<Collision>(movable_entity, 0.0f, false, sf::Rect<float>(0.0f, 0.0f, 50.0f, 50.0f));

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("src/assets/Ship/Ship.png")) {
        std::cerr << "Error loading texture" << std::endl;
        return -1;
    }

    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        system.control_system(reg);
        system.position_system(reg);
        system.loop_movement_system(reg);
        auto& positions = reg.get_components<Position>();
        for (std::size_t i = 0; i < positions.size(); ++i) {
            if (positions[i]) {
                if (positions[i]->x < 0) positions[i]->x = 0;
                    if (positions[i]->x > window.getSize().x - playerSprite.getGlobalBounds().width)
                    positions[i]->x = window.getSize().x - playerSprite.getGlobalBounds().width;
                if (positions[i]->y < 0) positions[i]->y = 0;
                if (positions[i]->y > window.getSize().y - playerSprite.getGlobalBounds().height)
                    positions[i]->y = window.getSize().y - playerSprite.getGlobalBounds().height;
            }
        }

        window.clear();
        system.draw_system(reg, window, playerSprite);
        window.display();
    }

    return 0;
}