#include "game.hpp"

int main() {
    rtype::Game game("R-Type", 1920, 1080, "assets/Ship/Ship.png");
    game.run();
    return 0;
}