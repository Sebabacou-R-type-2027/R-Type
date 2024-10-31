export module game:systems.buttons;
import :components.buttons;

import ecs;

using namespace ecs;
export namespace game::systems {
    void press_button(entity_container &ec, components::button &button, const ecs::components::position &position) noexcept
    {
        auto display = ec.get_entity(button.display).and_then([&ec](auto e){
            return ec.get_entity_component<ecs::components::gui::display>(e);
        });
        if (!display)
            return;
        if (!display->get().window->is_input_active(abstractions::gui::inputs::lclick))
            button.is_pressed = false;
        else if (ecs::abstractions::rectangle<float>(position, button.size).contains(display->get().window->get_cursor_position())
            && !button.is_pressed) {
            button.callback();
            button.is_pressed = true; // TODO: Prevent write on free'd memory if button is destroyed by callback
        } else
            button.is_pressed = false;
    }
}
