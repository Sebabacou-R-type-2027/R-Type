export module game:systems.shader_background;
import :components.settings;
import ecs;

using namespace ecs;
export namespace game::systems {

    /**
        * @brief Draw the shader background

        * This function is used to draw the shader background.

        * @param ec The entity container
        * @param settings The settings component
        * @param display The display component
     */
    void shader_background(entity_container &ec, components::settings &settings, const ecs::components::gui::display &display) noexcept
    {
        if (settings.isShaderEnabled) {
            display.window->draw_shader_background();
        }
    }
}
