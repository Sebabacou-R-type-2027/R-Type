export module game:systems.input_system;
import :components.input;

import ecs;
import utils;

#if __cpp_lib_modules >= 202207L
import std;
#endif

using namespace ecs;

export namespace game::systems {
    void handle_text_input(entity_container &ec, components::input &input, const ecs::components::position &position) noexcept
    {
        auto display = ec.get_entity(input.display).and_then([&ec](auto e){
            return ec.get_entity_component<ecs::components::gui::display>(e);
        });
        if (!display)
            return;
        if (input.has_focus) {
            if (display->get().window->is_input_active(abstractions::gui::inputs::period)) {
                input.content += ".";
            }


            if (display->get().window->is_input_active(abstractions::gui::inputs::enter)) {
                input.has_focus = false;
            }

            if (display->get().window->is_input_active(abstractions::gui::inputs::backspace)) {
                if (!input.content.empty()) {
                    input.content.pop_back();
                }
            }

            if (display->get().window->is_input_active(abstractions::gui::inputs::a)) {
                input.content += "a";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::b)) {
                input.content += "b";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::c)) {
                input.content += "c";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::d)) {
                input.content += "d";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::e)) {
                input.content += "e";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::f)) {
                input.content += "f";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::g)) {
                input.content += "g";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::h)) {
                input.content += "h";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::i)) {
                input.content += "i";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::j)) {
                input.content += "j";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::k)) {
                input.content += "k";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::l)) {
                input.content += "l";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::m)) {
                input.content += "m";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::n)) {
                input.content += "n";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::o)) {
                input.content += "o";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::p)) {
                input.content += "p";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::q)) {
                input.content += "q";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::r)) {
                input.content += "r";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::s)) {
                input.content += "s";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::t)) {
                input.content += "t";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::u)) {
                input.content += "u";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::v)) {
                input.content += "v";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::w)) {
                input.content += "w";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::x)) {
                input.content += "x";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::y)) {
                input.content += "y";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::z)) {
                input.content += "z";
            }

            if (display->get().window->is_input_active(abstractions::gui::inputs::zero) || display->get().window->is_input_active(abstractions::gui::inputs::numpad0)) {
                input.content += "0";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::one) || display->get().window->is_input_active(abstractions::gui::inputs::numpad1)) {
                input.content += "1";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::two) || display->get().window->is_input_active(abstractions::gui::inputs::numpad2)) {
                input.content += "2";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::three) || display->get().window->is_input_active(abstractions::gui::inputs::numpad3)) {
                input.content += "3";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::four) || display->get().window->is_input_active(abstractions::gui::inputs::numpad4)) {
                input.content += "4";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::five) || display->get().window->is_input_active(abstractions::gui::inputs::numpad5)) {
                input.content += "5";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::six) || display->get().window->is_input_active(abstractions::gui::inputs::numpad6)) {
                input.content += "6";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::seven) || display->get().window->is_input_active(abstractions::gui::inputs::numpad7)) {
                input.content += "7";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::eight) || display->get().window->is_input_active(abstractions::gui::inputs::numpad8)) {
                input.content += "8";
            }
            if (display->get().window->is_input_active(abstractions::gui::inputs::nine) || display->get().window->is_input_active(abstractions::gui::inputs::numpad9)) {
                input.content += "9";
            }
        }

        std::cout << input.content << std::endl;
    }
}
