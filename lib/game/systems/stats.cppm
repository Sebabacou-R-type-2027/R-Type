module;

#if __cpp_lib_modules < 202207L
#include <chrono>
#endif
#include <SFML/Graphics.hpp>
export module game:systems.stats;
import :components.stats;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

using namespace std::chrono_literals;

export namespace game::systems {
    void update_score(ecs::entity_container &ec, components::score &score, ecs::components::engine::hitbox &box)
    {
        if (!box.is_trigger) {
            return;
        }
        ec.get_entity_component<ecs::components::engine::hitbox>(score.game)->get().is_trigger = false;
        auto windowSize = ec.get_entity_component<ecs::components::gui::display>(score.game)->get().window->get_size();
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(score.game);
        const ecs::components::gui::display &display = *ec.get_entity_component<const ecs::components::gui::display>(score.game);
        auto score_text = ec.create_entity();
        ec.add_component(score_text, ecs::components::position{windowSize.x - 100.0f, 0.0f});
        ec.add_component(score_text, ecs::components::gui::drawable{score.game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {static_cast<ecs::entity>(score.game), display.factory->make_element(
                    "Score: " + std::to_string(score.value), asset_manager.get("arial"), 12)}
            })
        });
    }

    void update_life(ecs::entity_container &ec, components::health &life, ecs::components::engine::hitbox &box)
    {
        if (!box.is_trigger) {
            return;
        }

        box.is_trigger = false;
        life.value -= 1;

        if (life.value <= 0) {
            ec.erase_entity(life.game);
        }
    }
}
