/*
** EPITECH PROJECT, 2024
** ecs
** File description:
** components
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <string>

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

    Drawable(bool visible = true, sf::Sprite sprite = sf::Sprite(), sf::Texture texture = sf::Texture(),
             sf::IntRect texture_rect = sf::IntRect(), sf::Color color = sf::Color::White,
             float size = 50.0f, std::string shape = "square")
        : visible(visible), sprite(sprite), texture(texture), texture_rect(texture_rect),
          color(color), size(size), shape(shape) {}
};

// Component for entities controlled by user input
struct Controllable {
    bool active;
    float speed;
    std::string direction;

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
