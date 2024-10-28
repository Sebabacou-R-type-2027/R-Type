module;

#if __cpp_lib_modules < 202207L
#include <chrono>
#endif
#include <SFML/Graphics.hpp>
export module game:systems.projectiles;
import :components.projectiles;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

export namespace game::systems {
    void launch_projectile(ecs::entity_container &ec, components::projectile_launcher& launcher, const ecs::components::position& position) {
        auto now = std::chrono::steady_clock::now();
        if (now - launcher.last_shot < launcher.cooldown)
            return;

        using namespace std::chrono_literals;

        launcher.last_shot = now;
        auto projectile = ec.create_entity();
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(launcher.game);
        ec.add_component(projectile, components::projectile{10, now, 5s});
        ec.add_component(projectile, ecs::components::position{position.x, position.y});
        ec.add_component(projectile, ecs::components::engine::velocity{30.0f, 0.0f});
        ec.emplace_component<ecs::components::gui::drawable>(projectile,
            launcher.game, std::container<ecs::components::gui::drawable::elements_container>::make({
                {launcher.game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Sprite>(asset_manager.get_texture("bullet")))},
            })
        );
    }

    void cull_projectiles(ecs::entity e, ecs::entity_container &ec, const components::projectile &projectile) {
        auto now = std::chrono::steady_clock::now();
        if (now - projectile.birth > projectile.lifetime)
            ec.erase_entity(e);
    }
}
