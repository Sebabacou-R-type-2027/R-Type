#if __cpp_lib_modules < 202207L
module;

#include <chrono>
#endif
export module game:systems.projectiles;
import :components.projectiles;
import :components.stats;
import :components.enemies;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

export namespace game::systems {
    void launch_projectile(ecs::entity e, ecs::entity_container &ec, components::projectile_launcher& launcher, const ecs::components::position& position)
    {
        auto now = std::chrono::steady_clock::now();
        if (now - launcher.last_shot < launcher.cooldown)
            return;

        using namespace std::chrono_literals;

        launcher.last_shot = now;
        auto projectile = ec.create_entity();
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(launcher.game);
        const ecs::components::gui::display &display =
            *ec.get_entity_component<const ecs::components::gui::display>(launcher.game);
        ec.add_component(projectile, components::projectile{1, e, now, 5s});
        ec.add_component(projectile, ecs::components::position{position.x, position.y});
        ec.add_component(projectile, ecs::components::engine::velocity{50.0f * launcher.direction, 0.0f});
        ec.add_component(projectile, ecs::components::engine::hitbox{ecs::abstractions::rectangle<float>{position.x, position.y, 10.0f, 10.0f}});
        ec.emplace_component<ecs::components::gui::drawable>(projectile, ecs::components::gui::drawable{launcher.game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {static_cast<ecs::entity>(launcher.game), display.factory->make_element(
                    "bullet", asset_manager.get("arial"), 12)},
                {static_cast<ecs::entity>(launcher.game), display.factory->make_element(
                    dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("bullet")), {1, 1}, 10ms)}
            })
        });
    }

    void launch_many_projectiles(ecs::entity e, ecs::entity_container &ec, components::projectile_launcher &launcher, const ecs::components::position &position)
    {
        auto now = std::chrono::steady_clock::now();
        if (now - launcher.last_shot < launcher.cooldown)
            return;

        launcher.last_shot = now;

        int num_projectiles = 10;
        float angle_step = 20.0f;
        float initial_angle = -((num_projectiles - 1) / 2.0f) * angle_step;

        for (int i = 0; i < num_projectiles; ++i) {
            float angle = initial_angle + i * angle_step;
            float radian_angle = angle * std::numbers::pi / 180.0f;
            float velocity_x = std::cos(radian_angle) * 35.0f;
            float velocity_y = std::sin(radian_angle) * 35.0f;

            const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(launcher.game);
            const ecs::components::gui::display &display =
                *ec.get_entity_component<const ecs::components::gui::display>(launcher.game);
            auto projectile = ec.create_entity();
            ec.add_component(projectile, components::projectile{1, e, now, 5s});
            ec.add_component(projectile, ecs::components::position{position.x, position.y});
            ec.add_component(projectile, ecs::components::engine::velocity{velocity_x * launcher.direction, velocity_y});
            ec.add_component(projectile, ecs::components::engine::hitbox{
                ecs::abstractions::rectangle<float>{position.x, position.y, 10.0f, 10.0f}
            });
            ec.emplace_component<ecs::components::gui::drawable>(projectile, ecs::components::gui::drawable{launcher.game,
                std::container<ecs::components::gui::drawable::elements_container>::make({
                    {static_cast<ecs::entity>(launcher.game), display.factory->make_element(
                        "bullet", asset_manager.get("arial"), 12)},
                    {static_cast<ecs::entity>(launcher.game), display.factory->make_element(
                        dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("bullet")), {1, 1}, 10ms)}
                })
            });
        }
    }

    void launch_cross_projectiles(ecs::entity e, ecs::entity_container &ec, components::projectile_launcher &launcher, const ecs::components::position &position)
    {
        auto now = std::chrono::steady_clock::now();
        if (now - launcher.last_shot < launcher.cooldown)
            return;

        launcher.last_shot = now;

        std::array<std::pair<float, float>, 4> directions = {{
            {0.0f, -35.0f},
            {0.0f, 35.0f},
            {-35.0f, 0.0f},
            {35.0f, 0.0f}
        }};

        for (const auto &[vx, vy] : directions) {
            const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(launcher.game);
            const ecs::components::gui::display &display =
                *ec.get_entity_component<const ecs::components::gui::display>(launcher.game);
            auto projectile = ec.create_entity();
            ec.add_component(projectile, components::projectile{1, e, now, 5s});
            ec.add_component(projectile, ecs::components::position{position.x, position.y});
            ec.add_component(projectile, ecs::components::engine::velocity{vx * launcher.direction, vy});
            ec.add_component(projectile, ecs::components::engine::hitbox{
                ecs::abstractions::rectangle<float>{position.x, position.y, 10.0f, 10.0f}
            });
            ec.emplace_component<ecs::components::gui::drawable>(projectile, ecs::components::gui::drawable{launcher.game,
                std::container<ecs::components::gui::drawable::elements_container>::make({
                    {static_cast<ecs::entity>(launcher.game), display.factory->make_element(
                        "bullet", asset_manager.get("arial"), 12)},
                    {static_cast<ecs::entity>(launcher.game), display.factory->make_element(
                        dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("bullet")), {1, 1}, 10ms)}
                })
            });
        }
    }

    void launch_spiral_projectiles(ecs::entity e, ecs::entity_container &ec, components::projectile_launcher &launcher, const ecs::components::position &position)
    {
        auto now = std::chrono::steady_clock::now();
        if (now - launcher.last_shot < launcher.cooldown)
            return;

        launcher.last_shot = now;

        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(launcher.game);
        const ecs::components::gui::display &display = *ec.get_entity_component<const ecs::components::gui::display>(launcher.game);

        int num_projectiles = 30;
        float spiral_radius = 40.0f;
        float spiral_increment = 2.0f;
        float angle_increment = 15.0f;

        for (int i = 0; i < num_projectiles; ++i) {
            float angle = i * angle_increment;
            float radian_angle = angle * std::numbers::pi / 180.0f;
            float radius = spiral_radius + i * spiral_increment;

            float projectile_x = position.x + radius * std::cos(radian_angle);
            float projectile_y = position.y + radius * std::sin(radian_angle);

            auto projectile = ec.create_entity();
            ec.add_component(projectile, components::projectile{1, e, now, 5s});
            ec.add_component(projectile, ecs::components::position{projectile_x, projectile_y});
            ec.add_component(projectile, ecs::components::engine::velocity{35.0f * std::cos(radian_angle) * launcher.direction, 35.0f * std::sin(radian_angle)});
            ec.add_component(projectile, ecs::components::engine::hitbox{ecs::abstractions::rectangle<float>{projectile_x, projectile_y, 10.0f, 10.0f}});
            
            ec.emplace_component<ecs::components::gui::drawable>(projectile, ecs::components::gui::drawable{launcher.game,
                std::container<ecs::components::gui::drawable::elements_container>::make({
                    {static_cast<ecs::entity>(launcher.game), display.factory->make_element(
                        "bullet", asset_manager.get("arial"), 12)},
                    {static_cast<ecs::entity>(launcher.game), display.factory->make_element(
                        dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("bullet")), {1, 1}, 10ms)}
                })
            });
        }
    }

    void launch_projectile_ownership(ecs::entity e, ecs::entity_container &ec, components::projectile_launcher_ownership& launcher, const ecs::components::position& position)
    {
        auto now = std::chrono::steady_clock::now();
        if (now - launcher.last_shot < launcher.cooldown)
            return;

        using namespace std::chrono_literals;

        launcher.last_shot = now;
        const ecs::components::gui::display &display =
            *ec.get_entity_component<const ecs::components::gui::display>(launcher.game);
        if (!display.window)
            return;
        if (!display.window->is_open())
            return;
        if (display.window->is_input_active(ecs::abstractions::gui::inputs::space)) {
            if (!launcher.shot) {
                launcher.shot = true;
                auto projectile = ec.create_entity();
                const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(launcher.game);
                ec.add_component(projectile, components::projectile{1, e, now, 5s});
                ec.add_component(projectile, ecs::components::position{position.x, position.y});
                ec.add_component(projectile, ecs::components::engine::velocity{50.0f, 0.0f});
                ec.add_component(projectile, ecs::components::engine::hitbox{ecs::abstractions::rectangle<float>{position.x, position.y, 10.0f, 10.0f}});
                ec.emplace_component<ecs::components::gui::drawable>(projectile, ecs::components::gui::drawable{launcher.game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(launcher.game), display.factory->make_element(
                            "bullet", asset_manager.get("arial"), 12)},
                        {static_cast<ecs::entity>(launcher.game), display.factory->make_element(
                            dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("bullet")), {1, 1}, 10ms)}
                    })
                });
            } else {
                launcher.shot = false;
            }
        }
    }
    void cull_projectiles(ecs::entity e, ecs::entity_container &ec, const components::projectile &projectile) {
        auto now = std::chrono::steady_clock::now();
        if (now - projectile.birth > projectile.lifetime)
            ec.erase_entity(e);
    }
}
