#if __cpp_lib_modules < 202207L
module;

#include <chrono>
#endif
export module game:components.enemies;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

export namespace game::components {

    struct powerup_tripleshoot {
        bool tripleshoot; ///< Activate Triple Shoot
        std::chrono::steady_clock::time_point _birth;

        /**
            * @brief Constructor of the enemy component
        *
            * @param health Health of the entity
            * @param damage Damage of the entity
         */

        powerup_tripleshoot(bool tripleshoot, std::chrono::steady_clock::time_point birth)
            : tripleshoot(tripleshoot), _birth(birth) {}
    };
}