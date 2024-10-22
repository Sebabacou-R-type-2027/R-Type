/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system
*/

#pragma once

#include "control/control_system.hpp"
#include "draw/draw_system.hpp"
#include "position/position_system.hpp"
#include "animation/animation_system.hpp"
#include "collision/collision_system.hpp"
#include "../client/systems/loop_movement/loop_movement_system.hpp"
#include "../client/systems/button/button_system.hpp"
#include "../client/systems/bullet/bullet_system.hpp"
#include <networkClient/Client.hpp>
#include "../client/systems/shader/shader_system.hpp"
#include "../client/systems/shooting_enemy/shooting_enemy_system.hpp"
#include "../client/systems/chasing_enemy/chasing_enemy_system.hpp"
#include "../client/systems/spawner_enemy/spawner_enemy_system.hpp"

namespace ecs {
    /**
     * @brief Class that contains all the systems
     */
    class System {
    public:
        /**
         * @brief control_system
         *
         * @param registry contains all the logic of the components
         * @return void
         */
        void control_system(Registry& reg, client::Client& network) {
            controlSystem.update(reg, network, true);
        }

        /**
         * @brief position_system
         *
         * @param registry contains all the logic of the components
         * @return void
         */
        void position_system(Registry& reg) {
            positionSystem.update(reg);
        }

        /**
         * @brief draw_system
         *
         * @param registry contains all the logic of the components
         * @param window contains the window of the game
         * @return void
         */
        void draw_system(Registry& reg, sf::RenderWindow& window) {
            drawSystem.update(reg, window);
            buttonSystem.render(reg, window);
        }

        /**
         * @brief animation_system
         *
         * @param registry contains all the logic of the components
         * @param deltaTime contains the time between two frames
         * @param window contains the window of the game
         * @return void
         */
        void animation_system(Registry& reg, float deltaTime, sf::RenderWindow& window) {
            animationSystem.update(reg, deltaTime, window);
        }

        /**
         * @brief loop_movement_system
         *
         * @param registry contains all the logic of the components
         * @param deltaTime contains the time between two frames
         * @return void
         */
        void loop_movement_system(Registry& reg, float deltaTime) {
            loopMovementSystem.update(reg, deltaTime);
        }

        /**
         * @brief button_system
         *
         * @param registry contains all the logic of the components
         * @param window contains the window of the game
         * @return void
         */
        void button_system(Registry& reg, sf::RenderWindow& window) {
            buttonSystem.update(reg, window);
        }

        /**
         * @brief collision_system
         *
         * @param registry contains all the logic of the components
         * @param window contains the window of the game
         * @return void
         */
        void button_system_render(Registry& reg, sf::RenderWindow& window) {
            buttonSystem.render(reg, window);
        }

        /**
         * @brief collision_system
         *
         * @param registry contains all the logic of the components
         * @param window contains the window of the game
         * @return void
         */
        void collision_system(Registry& reg, sf::RenderWindow& window) {
            collisionSystem.update(reg, window);
        }

        /**
         * @brief shader_system
         *
         * @param registry contains all the logic of the components
         * @param window contains the window of the game
         * @param shader contains the shader of the game
         * @return void
         */
        void shader_system(Registry& reg, sf::RenderWindow& window, sf::Shader& shader) {
            shaderSystem.update(reg, window, shader);
        }

        /**
         * @brief shader_system_render
         *
         * @param registry contains all the logic of the components
         * @param window contains the window of the game
         * @param shader contains the shader of the game
         * @return void
         */
        void shader_system_render(Registry& reg, sf::RenderWindow& window, sf::Shader& shader) {
            shaderSystem.render(reg, window, shader);
        }

        /**
         * @brief shooting_enemy_system
         *
         * @param registry contains all the logic of the components
         * @param window contains the window of the game
         * @return void
         */
        void shooting_enemy_system(Registry& reg, sf::RenderWindow& window) {
            shootingEnemySystem.update(reg, window);
        }

        /**
         * @brief chasing_enemy_system
         *
         * @param registry contains all the logic of the components
         * @param window contains the window of the game
         * @return void
         */
        void chasing_enemy_system(Registry& reg, sf::RenderWindow& window) {
            chasingEnemySystem.update(reg, window);
        }

        /**
         * @brief spawner_enemy_system
         *
         * @param registry contains all the logic of the components
         * @param window contains the window of the game
         * @return void
         */
        void spawner_enemy_system(Registry& reg, sf::RenderWindow& window) {
            spawnerEnemySystem.update(reg, window);
        }

private:
    systems::ControlSystem controlSystem;
    systems::DrawSystem drawSystem;
    systems::PositionSystem positionSystem;
    systems::AnimationSystem animationSystem;
    systems::LoopMovementSystem loopMovementSystem;
    systems::ButtonSystem buttonSystem;
    systems::CollisionSystem collisionSystem;
    systems::ShaderSystem shaderSystem;
    systems::ShootingEnemySystem shootingEnemySystem;
    systems::ChasingEnemySystem chasingEnemySystem;
    systems::SpawnerEnemySystem spawnerEnemySystem;
};

}
