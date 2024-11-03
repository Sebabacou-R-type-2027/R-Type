module;

#if __cpp_lib_modules < 202207L
#include <chrono>
#endif
#include <SFML/Graphics.hpp>
export module game:systems.stats;
import :components.stats;
import :components.enemies;
import :components.projectiles;
import :components.powerup;


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
        if (!box.triggered_by) {
            return;
        }
        // FAIRE LE SYSTEME AVEC LE COMPONENT MARQUEUR
        //if (auto <components::health>(*box.trig)player = ec.get_entity_component<components::score>(*box.triggered_by)) {
            // if (ec.get_entity_component<components::composant_vide>(*box.triggered_by)) {}
            //ec.erase_entity(e);
            //std::cout <<"COUCOU" << std::endl;
        //}
        if (auto projectile = ec.get_entity_component<components::projectile>(*box.triggered_by)) {
            if (projectile->get().owner == e) {
                box.triggered_by = std::nullopt;
                return;
            }
            life.value -= projectile->get().damage;
            auto score = ec.get_entity_component<components::score>(projectile->get().owner);
            if (ec.get_entity_component<components::enemy>(e) && score) {
                score->get().value += ec.get_entity_component<components::enemy>(e)->get().points;
            }
            if (life.value <= 0) {
                ec.erase_entity(e);
            }
        }
        if (auto enemy = ec.get_entity_component<components::enemy>(*box.triggered_by)) {
            life.value -= enemy->get().damage;
            if (life.value <= 0) {
                ec.erase_entity(e);
            }
        }
    }
}
