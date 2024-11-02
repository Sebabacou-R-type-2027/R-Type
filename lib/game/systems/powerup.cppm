#if __cpp_lib_modules < 202207L
module;

#include <cmath>
#include <chrono>
#endif
export module game:systems.powerup;
import :components.projectiles;
import :components.powerup;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

using namespace std::chrono_literals;


export namespace game::systems {

    void triple_shoot_bonus(ecs::entity player, ecs::entity_container &ec, components::powerup_tripleshoot& powerup_tripleshoot, ecs::components::position& position)
    {
        auto now = std::chrono::steady_clock::now();
        // SI NOW DEPASSE LE DURATION EC.REMOVE COMPONENT
        // GAME.CPPM system register
        // TROUVER LE BON JOUEUR QUI A LE POWERUP -> ecs::entity (c'est l'entité qui a le system)
        //Sur component collisision → ajouter un composant power-up spawn
        
        if (now > powerup_tripleshoot.start + powerup_tripleshoot.duration) {
            std::cout << "powerupppppppp";
            ec.remove_component<components::powerup_tripleshoot>(player);
            return;
        }
        if (now - powerup_tripleshoot.last_shot < powerup_tripleshoot.cooldown)
            return;

        using namespace std::chrono_literals;

        auto projectile = ec.create_entity();
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(powerup_tripleshoot.game);
        const ecs::components::gui::display &display = *ec.get_entity_component<const ecs::components::gui::display>(powerup_tripleshoot.game);
        ec.add_component(projectile, components::projectile{10, now, 5s}); // POUR FAIRE LE HEAL METTRE JUSTE -10 sur le premier argument
        ec.add_component(projectile, ecs::components::position{position.x, position.y + 30});
        ec.add_component(projectile, ecs::components::engine::velocity{30.0f, 0.0f});
        ec.emplace_component<ecs::components::gui::drawable>(projectile,
            powerup_tripleshoot.game, std::container<ecs::components::gui::drawable::elements_container>::make({
                {powerup_tripleshoot.game, display.factory->make_element(
                    dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("bullet")))}
            })
        );
    }

}
