module;

#if __cpp_lib_modules < 202207L
#include <chrono>
#endif
#include <SFML/Graphics.hpp>
export module game:systems.stats;
import :components.stats;
import :components.enemies;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

using namespace std::chrono_literals;


export namespace game::systems {

    void update_score(ecs::entity_container &ec, components::score &score)
    {
        if (!ec.get_entity_component<components::score>(score.game)) {
            return;
        }
        auto windowSize = ec.get_entity_component<ecs::components::gui::display>(score.game)->get().window->get_size();
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(score.game);
        const ecs::components::gui::display &display = *ec.get_entity_component<const ecs::components::gui::display>(score.game);
        auto score_text = ec.create_entity();
        ec.add_component(score_text, ecs::components::position{windowSize.x - 200.0f, 0.0f});
        ec.add_component(score_text, ecs::components::gui::drawable{score.game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {static_cast<ecs::entity>(score.game), display.factory->make_element(
                    "Score: " + std::to_string(ec.get_entity_component<components::score>(score.game)->get().value), asset_manager.get("arial"), 24)}
            })
        });
    }

    void update_life(ecs::entity e, ecs::entity_container &ec, components::health &life, ecs::components::engine::hitbox &box)
    {
        if (!box.triggered) {
            return;
        }
        std::ranges::for_each(ec.get_entities(), [&](ecs::entity other) {
            if (e == other) {
                return;
            }
            if (ec.get_entity_component<components::id>(other)->get().value == ec.get_entity_component<components::id>(e)->get().value) {
                return;
            }
            if (ec.get_entity_component<components::enemy>(other) && ec.get_entity_component<components::enemy>(e)) {
                if (ec.get_entity_component<ecs::components::engine::hitbox>(other)->get().triggered == true && ec.get_entity_component<ecs::components::engine::hitbox>(e)->get().triggered == true) {
                    ec.get_entity_component<ecs::components::engine::hitbox>(other)->get().triggered = false;
                    ec.get_entity_component<ecs::components::engine::hitbox>(e)->get().triggered = false;
                    return;
                }
                return;
            }
        });
        box.triggered = false;
        life.value -= 1;

        if (life.value <= 0) {
            std::ranges::for_each(ec.get_entities(), [&](ecs::entity other) {
                if (e == other) {
                    return;
                }
                if (ec.get_entity_component<components::score>(other) && ec.get_entity_component<components::enemy>(e)) {
                    ec.get_entity_component<components::score>(other)->get().value = ec.get_entity_component<components::score>(other)->get().value + ec.get_entity_component<components::enemy>(e)->get().points;
                }
            });
            ec.erase_entity(e);
        }
    }
}
