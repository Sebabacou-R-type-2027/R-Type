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
#include "../Create_ennemies.hpp"
#include <networkClient/Client.hpp>
#include "../Handle_collision.hpp"
#include "../utils/FPSCounter.hpp"
#include "../utils/Settings.hpp"
#include "../systems/bullet/bullet_system.hpp"
#include "../systems/power-up/powerup_system.hpp"

namespace rtype {
    class GamePlayState : public GameState {
    public:
        GamePlayState(sf::RenderWindow& window, client::Client& network, bool isSolo);

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
        rtype::game::BulletSystem bulletSystem;
        rtype::game::PowerUp powerUpSystem;

        client::Client& network_;
        sf::View gameView;

        ecs::CreateEnnemies createEnnemies; // TODO: create a system for this
        ecs::HandleCollision handleCollision; // TODO: create a system for this

        void initPlayer(std::string path, float posx, bool me);
        void initChargeBullet();

        float calculateDeltaTime();
        void moveView(float deltaTime);
        void handlePlayerMovement(float deltaTime);
        void constrainPlayerPosition(std::optional<ecs::Position>& playerPos);
        void handle_mobs_wave(ecs::Registry& registry, sf::RenderWindow& window);
    };
}
