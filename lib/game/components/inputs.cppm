#if __cpp_lib_modules < 202207L
module;

#include <functional>
#endif
export module game:components.inputs;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

using namespace ecs::abstractions;

export namespace game::components {
    struct input {
        ecs::entity display;
        gui::text &text;
        std::string content = "";
        bool has_focus = false;
    };
}
