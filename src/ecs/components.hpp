/*
** EPITECH PROJECT, 2024
** ecs
** File description:
** components
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>


namespace ecs {

// Component for 2D position
struct Position {
    float x = 0;
    float y = 0;
    Position(float x = 0, float y = 0) : x(x), y(y) {}
};

// Component for velocity (speed in x and y directions)
struct Velocity {
    float vx = 0;
    float vy = 0;
    Velocity(float vx = 0, float vy = 0) : vx(vx), vy(vy) {}
};

// Component for entities that can be drawn
struct Drawable {
    bool visible;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect texture_rect;
    sf::Color color;
    float size;
    std::string shape;

    Drawable(const std::string& texturePath, bool visible = true,
             sf::IntRect texture_rect = sf::IntRect(), sf::Color color = sf::Color::White,
             float size = 50.0f, std::string shape = "square")
        : visible(visible), texture(), texture_rect(texture_rect),
          color(color), size(size), shape(shape) {
        if (!texture.loadFromFile(texturePath)) {
            // Handle error (log, assert, etc.)
            throw std::runtime_error("Failed to load texture from " + texturePath);
        }
        sprite.setTexture(texture);
    }
};

// Component for entities controlled by user input
struct Controllable {
    bool active;
    float speed;

    Controllable(bool active = true, float speed = 100.0f)
        : active(active), speed(speed) {}
};

struct Collision {
    float radius = 0;
    bool is_colliding = false;
    sf::Rect<float> rect;
    Collision(float radius = 0, bool is_colliding = false, sf::Rect<float> rect = sf::Rect<float>())
        : radius(radius), is_colliding(is_colliding), rect(rect) {}
};

struct LoopMovement {
    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float speed;
    LoopMovement(float min_x = 0.0f, float max_x = 0.0f, float min_y = 0.0f, float max_y = 0.0f, float speed = 0.0f)
        : min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y), speed(speed) {}
};

struct Acceleration {
    float ax = 0;
    float ay = 0;
    Acceleration(float ax = 0, float ay = 0) : ax(ax), ay(ay) {}
};

struct Button {
    sf::RectangleShape rect;
    sf::Text text;
    std::function<void()> onClick;
    bool isPressed = false;

    Button(const sf::RectangleShape& shape, const sf::Text& btnText, std::function<void()> action)
        : rect(shape), text(btnText), onClick(action) {
        rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
    }


};


}