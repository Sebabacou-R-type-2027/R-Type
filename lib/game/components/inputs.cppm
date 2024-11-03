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

    /**
        * @brief Component that defines the input
        * 
        * This component is used to define the input of the game. It contains the display entity, text component, content, and focus status.
     */
    struct input {
        ecs::entity display;
        gui::text &text;
        std::string content = "";
        bool has_focus = false;
    };
}
