/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system
*/

#pragma once

#include "control_system.hpp"
#include "draw_system.hpp"
#include "position_system.hpp"
#include "loop_movement_system.hpp"
#include "button_system.hpp"
#include "collision_system.hpp"
#include "animation_system.hpp"
#include "bullet_system.hpp"

namespace ecs {

class System {
public:
    void control_system(Registry& reg) {
        controlSystem.update(reg);
    }

    void position_system(Registry& reg) {
        positionSystem.update(reg);
    }

    void loop_movement_system(Registry& reg, float deltaTime) {
        loopMovementSystem.update(reg, deltaTime);
    }

    void draw_system(Registry& reg, sf::RenderWindow& window) {
        drawSystem.update(reg, window);
    }

    void button_system(Registry& reg, sf::RenderWindow& window) {
        buttonSystem.update(reg, window);
    }

    void button_system_render(Registry& reg, sf::RenderWindow& window) {
        buttonSystem.render(reg, window);
    }

    void collision_system(Registry& reg, sf::RenderWindow& window) {
        collisionSystem.update(reg, window);
    }

    void animation_system(Registry& reg, float deltaTime, sf::RenderWindow& window) {
        animationSystem.update(reg, deltaTime, window);
    }

    void bullet_system(Registry& reg) {
        bulletSystem.update(reg);
    }


private:
    systems::ControlSystem controlSystem;
    systems::DrawSystem drawSystem;
    systems::PositionSystem positionSystem;
    systems::LoopMovementSystem loopMovementSystem;
    systems::ButtonSystem buttonSystem;
    systems::CollisionSystem collisionSystem;
    systems::AnimationSystem animationSystem;
    systems::BulletSystem bulletSystem;
};

}
