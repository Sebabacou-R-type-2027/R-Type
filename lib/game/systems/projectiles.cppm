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

export namespace game::systems {
    void launch_projectile(ecs::entity_container &ec, components::projectile_launcher& launcher, const ecs::components::position& position) {
        auto now = std::chrono::steady_clock::now();
        if (now - launcher.last_shot < launcher.cooldown)
            return;

        using namespace std::chrono_literals;

        launcher.last_shot = now;
        auto projectile = ec.create_entity();
        ec.add_component(projectile, components::projectile{10, now, 3s});
        ec.add_component(projectile, ecs::components::position{position.x, position.y});
        ec.add_component(projectile, ecs::components::engine::velocity{10.0f, 0.0f});
        ec.emplace_component<ecs::components::gui::drawable>(projectile, ecs::components::gui::drawable{launcher.game,
            std::initializer_list<ecs::components::gui::drawable::elements_container::value_type>{
                {launcher.game, {
                    std::make_shared<sf::Text>("Pew",
                        ec.get_entity_component<ecs::components::gui::asset_manager>(launcher.game)->get().get_font("arial"), 12),
                        "arial"
                    }
                }
            }
        });
    }

    void cull_projectiles(ecs::entity e, ecs::entity_container &ec, const components::projectile &projectile) {
        auto now = std::chrono::steady_clock::now();
        if (now - projectile.birth > projectile.lifetime)
            ec.erase_entity(e);
    }
}
