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
#include <networkClient/Client.hpp>
#include "../Handle_collision.hpp"
#include "../systems/bullet/bullet_system.hpp"
#include "create_enemies.hpp"
#include "../systems/power-up/powerup_system.hpp"
#include "Game.hpp"

namespace rtype {
    class GamePlayState : public GameState {
    public:
        GamePlayState(sf::RenderWindow& window, client::Client& network , bool isSolo, Game &game);

        void handleInput() override;
        void update() override;
        void render() override;

    private:
        bool isSolo_;
        sf::RenderWindow& window;
        ecs::Registry registry;
        ecs::System system;
        sf::Shader backgroundShader;
        sf::Font font;
        Game& game; ///< Reference to the game object
        rtype::game::BulletSystem bulletSystem;
        rtype::game::PowerUp powerUpSystem;

        client::Client& network_;
        sf::View gameView;

        ecs::CreateEnemies createEnemies;
        ecs::HandleCollision handleCollision; // TODO: create a system for this

        void initPlayer(std::string path, float posx, bool me);
        void initChargeBullet();

        float calculateDeltaTime();
        void moveView(float deltaTime);
        void handlePlayerMovement(float deltaTime);
        void constrainPlayerPosition(std::optional<ecs::Position>& playerPos);
        void handle_mobs_wave(ecs::Registry& registry, sf::RenderWindow& window);
        void update_score(ecs::Registry& registry, client::Client& network, bool isSolo);
    };
}
