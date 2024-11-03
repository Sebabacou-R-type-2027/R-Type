export module game:systems.controllable_network;
import :components.network;

import ecs;
import std;

using namespace ecs;
export namespace game::systems {

constexpr void control(ecs::entity e, entity_container &ec, ecs::components::position &pos, const ecs::components::engine::controllable &c) noexcept
{
if (!c.enabled)
return;

        auto display = ec.get_entity(c.display).and_then([&ec](auto e){
            return ec.get_entity_component<ecs::components::gui::display>(e);
        });
        if (!display)
            return;

        auto network = ec.get_entity_component<components::network>(c.display);
        auto p = ec.get_entity_component<ecs::components::engine::controllable>(e);

        if (network->get().client->is_connected()) {
            if (display->get().window->is_input_active(ecs::abstractions::gui::inputs::left))
                network->get().client->send_message("CMDP|1");
            if (display->get().window->is_input_active(ecs::abstractions::gui::inputs::right))
                network->get().client->send_message("CMDP|2");
            if (display->get().window->is_input_active(ecs::abstractions::gui::inputs::up))
                network->get().client->send_message("CMDP|3");
            if (display->get().window->is_input_active(ecs::abstractions::gui::inputs::down))
                network->get().client->send_message("CMDP|4");

            for (auto it = network->get().client->_commandsToDo.begin(); it != network->get().client->_commandsToDo.end(); ) {
                if (it->first == std::to_string(p->get().network_id)) {
                    if (it->second == "1")
                        pos.x -= c.speed;
                    if (it->second == "2")
                        pos.x += c.speed;
                    if (it->second == "3")
                        pos.y -= c.speed;
                    if (it->second == "4")
                        pos.y += c.speed;
                    if (it->second == "0")
                      	return;
                    it = network->get().client->_commandsToDo.erase(it);
                } else {
                    ++it;
                }
            }
        } else {
            if (display->get().window->is_input_active(abstractions::gui::inputs::left))
                pos.x -= c.speed;
            if (display->get().window->is_input_active(abstractions::gui::inputs::right))
                pos.x += c.speed;
            if (display->get().window->is_input_active(abstractions::gui::inputs::up))
                pos.y -= c.speed;
            if (display->get().window->is_input_active(abstractions::gui::inputs::down))
                pos.y += c.speed;
        }
    }
}
