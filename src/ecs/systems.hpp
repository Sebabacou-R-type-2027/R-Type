/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** systems
*/

#pragma once

#include "registry.hpp"
#include "components.hpp"
#include <SFML/Graphics.hpp>

void position_system(Registry& registry);
void control_system(Registry& registry);
void draw_system(Registry& registry, sf::RenderWindow& window, sf::CircleShape& playerShape);
void draw_system(Registry& registry, sf::RenderWindow& window, sf::Sprite& playerSprite);
