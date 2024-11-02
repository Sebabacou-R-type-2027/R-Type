export module game:systems.inputs;
import :components.buttons;
import :components.inputs;

#if __cpp_lib_modules >= 202207L
import std;
#endif
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
    {ecs::abstractions::gui::inputs::z, 'z'}
};

export namespace game::systems {
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
