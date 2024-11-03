#if __cpp_lib_modules < 202207L
module;

#include <functional>
#include <string>
#include <vector>
#include <map>
#endif
export module game:components.spawn_waves;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

using namespace ecs::abstractions;

export namespace game::components {
    struct spawn_waves {
        ecs::entity game;
        std::string_view path;
        int current_wave;
    };
}