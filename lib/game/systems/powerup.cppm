#if __cpp_lib_modules < 202207L
module;

#include <cmath>
#include <chrono>
#endif
export module game:systems.powerup;
import :components.projectiles;
import :components.powerup;
import :components.enemies;

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
            ec.remove_component<components::powerup_tripleshoot>(player);
            return;
        }

        if (now - powerup_tripleshoot.last_shot < powerup_tripleshoot.cooldown)
            return;

        using namespace std::chrono_literals;
        powerup_tripleshoot.last_shot = now;

        //ENTITY PROJECTILE BOT
        auto projectile_bot = ec.create_entity();
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(powerup_tripleshoot.game);
        const ecs::components::gui::display &display = *ec.get_entity_component<const ecs::components::gui::display>(powerup_tripleshoot.game);
        ec.add_component(projectile_bot, components::projectile{10, now, 5s}); // POUR FAIRE LE HEAL METTRE JUSTE -10 sur le premier argument
        ec.add_component(projectile_bot, ecs::components::position{position.x, position.y + 35});
        ec.add_component(projectile_bot, ecs::components::engine::velocity{30.0f, 0.0f});
        ec.emplace_component<ecs::components::gui::drawable>(projectile_bot,
            powerup_tripleshoot.game, std::container<ecs::components::gui::drawable::elements_container>::make({
                {powerup_tripleshoot.game, display.factory->make_element(
                    dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("bullet")))}
            })
        );

        //ENTITY PROJECTILE TOP
        auto projectile_top = ec.create_entity();
        ec.add_component(projectile_top, components::projectile{10, now, 5s}); // POUR FAIRE LE HEAL METTRE JUSTE -10 sur le premier argument
        ec.add_component(projectile_top, ecs::components::position{position.x, position.y - 35});
        ec.add_component(projectile_top, ecs::components::engine::velocity{30.0f, 0.0f});
        ec.emplace_component<ecs::components::gui::drawable>(projectile_top,
            powerup_tripleshoot.game, std::container<ecs::components::gui::drawable::elements_container>::make({
                {powerup_tripleshoot.game, display.factory->make_element(
                    dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("bullet")))}
            })
        );
    }



    void heal_shoot_bonus(ecs::entity player, ecs::entity_container &ec, components::powerup_curveshoot& powerup_curveshoot, ecs::components::position& position)
    {
        // Quand il est activé remove le component bullet classic et
        auto now = std::chrono::steady_clock::now();
        // SI NOW DEPASSE LE DURATION EC.REMOVE COMPONENT
        // GAME.CPPM system register
        // TROUVER LE BON JOUEUR QUI A LE POWERUP -> ecs::entity (c'est l'entité qui a le system)
        //Sur component collisision → ajouter un composant power-up spawn

        if (now > powerup_curveshoot.start + powerup_curveshoot.duration) {
            ec.remove_component<components::powerup_curveshoot>(player);
            return;
        }

        if (now - powerup_curveshoot.last_shot < powerup_curveshoot.cooldown)
            return;

        using namespace std::chrono_literals;
        powerup_curveshoot.last_shot = now;

        //ENTITY PROJECTILE BOT
        auto projectile_bot = ec.create_entity();
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(powerup_curveshoot.game);
        const ecs::components::gui::display &display = *ec.get_entity_component<const ecs::components::gui::display>(powerup_curveshoot.game);
        ec.add_component(projectile_bot, components::projectile{10, now, 5s}); // POUR FAIRE LE HEAL METTRE JUSTE -10 sur le premier argument
        ec.add_component(projectile_bot, ecs::components::position{position.x, position.y + 35});
        ec.add_component(projectile_bot, ecs::components::engine::velocity{30.0f, 0.0f});


        ec.add_component(projectile_bot, components::enemy_loop_movement{0.0f, 2000.0f, 200.0f, 250.0f, 1.0f, 0.0f, 100.0f, 2.0f});

        ec.emplace_component<ecs::components::gui::drawable>(projectile_bot,
            powerup_curveshoot.game, std::container<ecs::components::gui::drawable::elements_container>::make({
                {powerup_curveshoot.game, display.factory->make_element(
                    dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("bullet_heal")))}
            })
        );
    }

}
