/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Animation
*/

#pragma once
#include <iostream>

namespace ecs {
    struct Animation {
        sf::Sprite sprite;
        sf::Vector2i imageCount;
        sf::Vector2i currentImage;
        sf::Vector2i imageSize;
        float switchTime;
        float totalTime;

        Animation(const std::string& spriteSheetPath = "", int imageCountX = 0, int imageCountY = 0, float switchTime = 0.0f)
            : imageCount(imageCountX, imageCountY), currentImage(0, 0), switchTime(switchTime), totalTime(0.0f) {
            static sf::Texture sharedTexture;
            if (!sharedTexture.loadFromFile(spriteSheetPath)) {
                throw std::runtime_error("Failed to load sprite sheet");
            } else {
                std::cout << "Loaded sprite sheet" << std::endl;
            }

            imageSize = sf::Vector2i(sharedTexture.getSize().x / imageCount.x, sharedTexture.getSize().y / imageCount.y);

            sprite.setTexture(sharedTexture);
            // sprite.setScale(2.0f, 2.0f);
        }

        void update(float deltaTime) {
            totalTime += deltaTime;

            if (totalTime >= switchTime) {
                totalTime -= switchTime;

                currentImage.x++;
                if (currentImage.x >= imageCount.x) {
                    currentImage.x = 0;
                    currentImage.y++;

                    if (currentImage.y >= imageCount.y) {
                        currentImage.y = 0;
                    }
                }
                sprite.setTextureRect(sf::IntRect(currentImage.x * imageSize.x,
                                                currentImage.y * imageSize.y,
                                                imageSize.x,
                                                imageSize.y));
            }
        }
    };
}
