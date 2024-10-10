/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "registry.hpp"
#include "create_ennemies.hpp"
#include "system.hpp"

#include "bullet_event.hpp"
#include "game_state_manager.hpp"
#include "utils/FPSCounter.hpp"
#include "factories/button_factory.hpp"

namespace rtype {
    class Game {
        public:
            Game(const std::string& title, unsigned int width, unsigned int height, const std::string& playerTexturePath);
            void run();
        private:
            void initChargeBullet();
            void processEvents(GameStateManager& gameStateManager);
            void update(rtype::game::BulletSystem& bulletSystem);
            void render();
            void renderMenu();
            void renderPauseScreen();
            void renderGameOverScreen();
            void renderGame();
            void startGame();
            void createMenuButtons(sf::Font& font);

            // SFML
            sf::RenderWindow window;
            sf::Sprite playerSprite;
            sf::Texture playerTexture;

            // Game State
            GameStateManager gameStateManager;
            rtype::game::BulletSystem bulletSystem;

            // ECS
            ecs::CreateEnnemies createEnnemies;
            ecs::Registry registry;
            ecs::System system;

            // FPS Counter
            sf::Text fpsText;
            FPSCounter fpsCounter;
    };
}

