/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** animation_system
*/

#pragma once

#include "registry.hpp"
#include <SFML/Graphics.hpp>

namespace ecs::systems {

    class AnimationSystem {
        public:
            AnimationSystem() = default;
            void update(Registry& registry, float deltaTime, sf::RenderWindow& window);
            void setPos(sf::Sprite &sprite, float x, float y);
            void setTexture(sf::Sprite &sprite, sf::Texture &texture);
            void setScale(sf::Sprite &sprite, float x, float y);
            void setOrigin(sf::Sprite &sprite, float x, float y);
            void setRotation(sf::Sprite &sprite, float angle);
            void setTextureRect(sf::Sprite &sprite, int x, int y, int width, int height);
            void draw(sf::RenderWindow& window, sf::Sprite &sprite);
        private:
            float _time = 0.0f;
            float _switchTime = 0.1f;
    };

}