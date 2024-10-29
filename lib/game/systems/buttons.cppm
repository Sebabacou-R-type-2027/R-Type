module;

#include <SFML/Graphics.hpp>
export module game:systems.buttons;
import :components.buttons;
#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

using namespace ecs;

export namespace game::systems {
    void press_button(entity_container &ec, components::button &button) noexcept
    {
        auto window = ec.get_entity(button.asset_manager).and_then([&ec](auto e){
            return ec.get_entity_component<gui::window>(e);
        });
        if (window)
            button.update(sf::Vector2f(sf::Mouse::getPosition(*window->get().window)));
    }
}
