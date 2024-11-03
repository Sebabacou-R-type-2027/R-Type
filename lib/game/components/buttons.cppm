export module game:components.buttons;

import std;
import ecs;

using namespace ecs::abstractions;

export namespace game::components {

    /**
        * @brief Component that defines the button

        * This component is used to define the button of the game. It contains the display entity, size, and callback of the entity.
     */
    struct button {
        ecs::entity display;
        vector<float> size;
        std::function<void()> callback;
        bool is_pressed = false;
    };
}
