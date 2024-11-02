#if __cpp_lib_modules < 202207L
module;

#include <chrono>
#include <chrono>

#endif
export module game:components.powerup;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

export namespace game::components {

    struct powerup_tripleshoot {
        const std::chrono::steady_clock::duration cooldown;
        std::chrono::steady_clock::time_point last_shot;
        ecs::entity game;
        const std::chrono::steady_clock::duration duration;
        std::chrono::steady_clock::time_point start;
    };
}