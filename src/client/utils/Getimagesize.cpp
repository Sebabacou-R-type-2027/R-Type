#include "Getimagesize.hpp"

sf::Vector2i get_image_size(std::string pathTexture)
{
    static sf::Texture sharedTexture;
    if (!sharedTexture.loadFromFile(pathTexture)) {
        throw std::runtime_error("Failed to load sprite sheet");
    } else {
        std::cout << "Loaded sprite sheet" << std::endl;
    }
    sf::Vector2i imageSize = sf::Vector2i(sharedTexture.getSize().x, sharedTexture.getSize().y);
    return imageSize;
};

