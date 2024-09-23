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

class System {
    public:
        void control_system(Registry& reg) {
            controlSystem.control_system(reg);
        }

        void position_system(Registry& reg) {
            positionSystem.position_system(reg);
        }

        void loop_movement_system(Registry& reg) {
            loopMovementSystem.loop_movement_system(reg);
        }

        void draw_system(Registry& reg, sf::RenderWindow& window, sf::Sprite& playerSprite) {
            drawSystem.draw_system(reg, window, playerSprite);
        }

    private:
        ControlSystem controlSystem;
        DrawSystem drawSystem;
        PositionSystem positionSystem;
        LoopMovementSystem loopMovementSystem;
    };