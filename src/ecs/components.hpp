/*
** EPITECH PROJECT, 2024
** ecs
** File description:
** components
*/

#pragma once

// Component for 2D position
struct Position {
    float x = 0;
    float y = 0;
};

// Component for velocity (speed in x and y directions)
struct Velocity {
    float vx = 0;
    float vy = 0;
};

// Component for entities that can be drawn
struct Drawable {
    // Placeholder for drawing logic (e.g., a sprite reference)
    // For now, it's just a marker component
};

// Component for entities controlled by user input
struct Controllable {
    // Placeholder for input logic (e.g., control scheme)
    // For now, it's just a marker component
};
