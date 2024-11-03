export module game:systems.shader_background;
import :components.settings;
import ecs;

using namespace ecs;
export namespace game::systems {
    void shader_background(entity_container &ec, components::settings &settings, const ecs::components::gui::display &display) noexcept
    {
        if (settings.isShaderEnabled) {
            display.window->draw_shader_background();
        }
    }
}
