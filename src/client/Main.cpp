/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "Game.hpp"

int main() {
    rtype::Game game("R-Type", 1920, 1080, "assets/Ship/Ship.png");
    game.run();
    return 0;
}