#include <SFML/Graphics.hpp>
#include <iostream>

#include "registry.hpp"
#include "components.hpp"
#include "systems.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Game");
    window.setFramerateLimit(60);

    Registry registry;

    registry.register_component<Position>();
    registry.register_component<Velocity>();
    registry.register_component<Drawable>();
    registry.register_component<Controllable>();

    Entity player = registry.spawn_entity();

    registry.add_component<Position>(player, Position{400.0f, 300.0f});
    registry.add_component<Velocity>(player, Velocity{0.0f, 0.0f});
    registry.add_component<Drawable>(player, Drawable{});
    registry.add_component<Controllable>(player, Controllable{});

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

        control_system(registry);

        position_system(registry);

        auto& positions = registry.get_components<Position>();
        for (size_t i = 0; i < positions.size(); ++i) {
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
        draw_system(registry, window, playerSprite);
        window.display();
    }

    return 0;
}