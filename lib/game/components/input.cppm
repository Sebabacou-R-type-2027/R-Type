#if __cpp_lib_modules < 202207L
module;

#include <functional>
#endif
export module game:components.input;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

using namespace ecs::abstractions;

export namespace game::components {
    struct input {
        bool has_focus = false;
        std::string content;
        ecs::entity display;
        ecs::entity text;

    };
}