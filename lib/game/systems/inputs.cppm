export module game:systems.inputs;
import :components.buttons;
import :components.inputs;

import std;
import ecs;
import utils;

static const std::unordered_map<ecs::abstractions::gui::inputs, char> characters = {
    {ecs::abstractions::gui::inputs::a, 'a'},
    {ecs::abstractions::gui::inputs::b, 'b'},
    {ecs::abstractions::gui::inputs::c, 'c'},
    {ecs::abstractions::gui::inputs::d, 'd'},
    {ecs::abstractions::gui::inputs::e, 'e'},
    {ecs::abstractions::gui::inputs::f, 'f'},
    {ecs::abstractions::gui::inputs::g, 'g'},
    {ecs::abstractions::gui::inputs::h, 'h'},
    {ecs::abstractions::gui::inputs::i, 'i'},
    {ecs::abstractions::gui::inputs::j, 'j'},
    {ecs::abstractions::gui::inputs::k, 'k'},
    {ecs::abstractions::gui::inputs::l, 'l'},
    {ecs::abstractions::gui::inputs::m, 'm'},
    {ecs::abstractions::gui::inputs::n, 'n'},
    {ecs::abstractions::gui::inputs::o, 'o'},
    {ecs::abstractions::gui::inputs::p, 'p'},
    {ecs::abstractions::gui::inputs::q, 'q'},
    {ecs::abstractions::gui::inputs::r, 'r'},
    {ecs::abstractions::gui::inputs::s, 's'},
    {ecs::abstractions::gui::inputs::t, 't'},
    {ecs::abstractions::gui::inputs::u, 'u'},
    {ecs::abstractions::gui::inputs::v, 'v'},
    {ecs::abstractions::gui::inputs::w, 'w'},
    {ecs::abstractions::gui::inputs::x, 'x'},
    {ecs::abstractions::gui::inputs::y, 'y'},
    {ecs::abstractions::gui::inputs::z, 'z'},
    {ecs::abstractions::gui::inputs::one, '1'},
    {ecs::abstractions::gui::inputs::two, '2'},
    {ecs::abstractions::gui::inputs::three, '3'},
    {ecs::abstractions::gui::inputs::four, '4'},
    {ecs::abstractions::gui::inputs::five, '5'},
    {ecs::abstractions::gui::inputs::six, '6'},
    {ecs::abstractions::gui::inputs::seven, '7'},
    {ecs::abstractions::gui::inputs::eight, '8'},
    {ecs::abstractions::gui::inputs::nine, '9'},
    {ecs::abstractions::gui::inputs::zero, '0'},
    {ecs::abstractions::gui::inputs::numpad1, '1'},
    {ecs::abstractions::gui::inputs::numpad2, '2'},
    {ecs::abstractions::gui::inputs::numpad3, '3'},
    {ecs::abstractions::gui::inputs::numpad4, '4'},
    {ecs::abstractions::gui::inputs::numpad5, '5'},
    {ecs::abstractions::gui::inputs::numpad6, '6'},
    {ecs::abstractions::gui::inputs::numpad7, '7'},
    {ecs::abstractions::gui::inputs::numpad8, '8'},
    {ecs::abstractions::gui::inputs::numpad9, '9'},
    {ecs::abstractions::gui::inputs::numpad0, '0'},
    {ecs::abstractions::gui::inputs::period, '.'},
};

export namespace game::systems {

    /**
        * @brief Handle text input
        *
        * This function is used to handle the text input.
        *
        * @param ec The entity container
        * @param input The input component
     */
    void handle_text_input(ecs::entity_container &ec, components::input &input) noexcept
    {
        auto display = ec.get_entity(input.display).and_then([&ec](auto e){
            return ec.get_entity_component<ecs::components::gui::display>(e);
        });
        if (!display || !input.has_focus)
            return;
        if (display->get().window->is_input_active(ecs::abstractions::gui::inputs::enter))
            input.has_focus = false;
        else if (display->get().window->is_input_active(ecs::abstractions::gui::inputs::backspace)
            && !input.content.empty())
            input.content.pop_back();
        else std::ranges::for_each(characters | std::views::keys, [&](auto key){
            if (display->get().window->is_input_active(key))
                input.content += characters.at(key);
        });
        input.text.set_text(input.content);
    }

    /**
        * @brief Reset focus
        *
        * This function is used to reset the focus of the input component.
        *
        * @param e The entity
        * @param ec The entity container
        * @param input The input component
        * @param button The button component
     */
    void reset_focus(ecs::entity e, ecs::entity_container &ec,
        components::input &input, components::button &button) noexcept
    {
        if (!(input.has_focus && button.is_pressed))
            return;
        std::ranges::for_each(ec.get_entities(), [input = e, &ec](auto e){
            if (input != e)
                if (auto other = ec.get_entity_component<components::input>(e))
                    other->get().has_focus = false;
        });
    }
}
