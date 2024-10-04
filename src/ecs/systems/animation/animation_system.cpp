/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** animation_system
*/

#include "animation_system.hpp"
#include <chrono>

namespace ecs::systems {

    void AnimationSystem::update(Registry& registry, float deltaTime, sf::RenderWindow& window) {
        auto &animations = registry.get_components<Animation>();

        for (std::size_t i = 0; i < animations.size(); ++i) {
            if (animations[i]) {
                auto &animation = *animations[i];

                _time += deltaTime;
                if (_time >= _switchTime) {
                    _time -= _switchTime;
                    animation.currentImage.x++;
                    if (animation.currentImage.x >= animation.imageCount.x) {
                        animation.currentImage.x = 0;
                        animation.currentImage.y++;
                        if (animation.currentImage.y >= animation.imageCount.y) {
                            animation.currentImage.y = 0;
                        }
                    }
                }
                setTextureRect(animation.sprite, animation.currentImage.x * animation.imageSize.x,
                               animation.currentImage.y * animation.imageSize.y, animation.imageSize.x,
                               animation.imageSize.y);
                draw(window, animation.sprite);
            }
        }
    }

    void AnimationSystem::setPos(sf::Sprite &sprite, float x, float y) {
        sprite.setPosition(x, y);
    }

    void AnimationSystem::setTexture(sf::Sprite &sprite, sf::Texture &texture) {
        sprite.setTexture(texture);
    }

    void AnimationSystem::setScale(sf::Sprite &sprite, float x, float y) {
        sprite.setScale(x, y);
    }

    void AnimationSystem::setOrigin(sf::Sprite &sprite, float x, float y) {
        sprite.setOrigin(x, y);
    }

    void AnimationSystem::setRotation(sf::Sprite &sprite, float angle) {
        sprite.setRotation(angle);
    }

    void AnimationSystem::setTextureRect(sf::Sprite &sprite, int x, int y, int width, int height) {
        sprite.setTextureRect(sf::IntRect(x, y, width, height));
    }

    void AnimationSystem::draw(sf::RenderWindow& window, sf::Sprite &sprite) {
        window.draw(sprite);
    }

}