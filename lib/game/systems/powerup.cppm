module;

#if __cpp_lib_modules < 202207L
#include <cmath>
#include <chrono>
#endif
#include <SFML/Graphics.hpp>
export module game:systems.enemies;
import :components.enemies;
import :components.projectiles;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

using namespace std::chrono_literals;


export namespace game::systems {
    void move_powerup(ecs::entity_container &ec, game::components::powerup_tripleshoot &chaser, ecs::components::position& position, float time)
    {
        const float amplitude = 5.0f;
        const float frequency = 1.0f;
        position.y = position.y + amplitude * std::sin(frequency * time);
    }

}