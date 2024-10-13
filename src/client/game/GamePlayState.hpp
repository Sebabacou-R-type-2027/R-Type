/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GamePlayState
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "game_state.hpp"
#include "registry.hpp"
#include "system.hpp"
#include "../create_ennemies.hpp"
#include "../utils/FPSCounter.hpp"
#include "../utils/Settings.hpp"
#include "../systems/bullet/bullet_system.hpp"

namespace rtype {
    class GamePlayState : public GameState {
    public:
        GamePlayState(sf::RenderWindow& window);

        void handleInput() override;
        void update() override;
        void render() override;

    private:
        sf::RenderWindow& window;
        ecs::Registry registry;
        ecs::System system;
        sf::Shader backgroundShader;
        sf::Font font;
        rtype::game::BulletSystem bulletSystem;

        sf::View gameView;

        ecs::CreateEnnemies createEnnemies; // TODO: create a system for this

        void initPlayer(std::string path);
        void initChargeBullet();

        float calculateDeltaTime();
        void moveView(float deltaTime);
        void handlePlayerMovement(float deltaTime);
        void constrainPlayerPosition(std::optional<ecs::Position>& playerPos);
    };
}
