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

namespace ecs {

class System {
public:
    void control_system(Registry& reg) {
        controlSystem.update(reg);
    }

    void position_system(Registry& reg) {
        positionSystem.update(reg);
    }

    void loop_movement_system(Registry& reg) {
        loopMovementSystem.update(reg);
    }

    void draw_system(Registry& reg, sf::RenderWindow& window, sf::Sprite& playerSprite) {
        drawSystem.update(reg, window, playerSprite);
    }

private:
    systems::ControlSystem controlSystem;
    systems::DrawSystem drawSystem;
    systems::PositionSystem positionSystem;
    systems::LoopMovementSystem loopMovementSystem;
};

}
