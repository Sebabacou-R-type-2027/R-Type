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
    void update_score(ecs::entity_container &ec, components::score &score)
    {
        if (ec.get_entity_component<ecs::components::lifestate>(score.game)->get().alive)
            return;
        auto windowSize = ec.get_entity_component<ecs::components::gui::window>(score.game)->get().window->getSize();
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(score.game);
        auto score_text = std::make_unique<sf::Text>("Score: " + std::to_string(score.value), asset_manager.get_font("arial"));
        score_text->setPosition(100, 25);
        std::cout << "Score: " << score.value << std::endl;
        ec.emplace_component<ecs::components::gui::drawable>(score.game, ecs::components::gui::drawable{score.game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {static_cast<ecs::entity>(score.game), std::make_unique<ecs::components::gui::display_element>(
                    std::move(score_text), "arial")}
            })
        });
    }

    void update_life(ecs::entity_container &ec, components::health &health)
    {
        if (ec.get_entity_component<ecs::components::lifestate>(health.game)->get().alive)
            return;
        ec.get_entity_component<ecs::components::health>(health.game)->get().value -= 1;

        if (ec.get_entity_component<ecs::components::health>(health.game)->get().value <= 0) {
            ec.erase_entity(health.game);
        }
    }
}
