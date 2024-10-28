module;

#if __cpp_lib_modules < 202207L
#include <cmath>
#include <chrono>
#endif
#include <SFML/Graphics.hpp>
export module game:systems.enemies;
import :components.enemies;
import :components.projectiles;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

using namespace std::chrono_literals;

export namespace game::systems {

    void move_enemy_chaser(ecs::entity_container &ec, game::components::enemy_chaser &chaser, ecs::components::position& position)
    {
        auto target_position = ec.get_entity_component<ecs::components::position>(chaser._target);
        auto target_velocity = ec.get_entity_component<ecs::components::engine::velocity>(chaser._target);

        float dx = target_position->get().x - position.x;
        float dy = target_position->get().y - position.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < 1.0f)
            return;

        float vx = dx / distance * chaser._speed;
        float vy = dy / distance * chaser._speed;

        position.x += vx;
        position.y += vy;
    }

    void move_enemy_shooter(ecs::entity_container &ec, game::components::enemy_shooter &shooter, const ecs::components::position& position)
    {
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(shooter.game);
        auto now = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - shooter.last_update);

        if (elapsed_time < std::chrono::milliseconds(static_cast<long>(shooter.cooldown * 1000)))
            return;

        shooter.last_update = now;

        using namespace std::chrono_literals;
        auto projectile = ec.create_entity();
        ec.add_component(projectile, components::projectile{10, now, 3s});
        ec.add_component(projectile, ecs::components::position{position.x, position.y});
        ec.add_component(projectile, ecs::components::engine::velocity{10.0f, 10.0f});
        ec.emplace_component<ecs::components::gui::drawable>(projectile,
            shooter.game, std::container<ecs::components::gui::drawable::elements_container>::make({
                {shooter.game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Pew", asset_manager.get_font("arial"), 12), "arial")}
            })
        );
    }

    void handle_enemy_spawner(ecs::entity_container &ec, game::components::enemy_spawner &spawner, ecs::components::position& position)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - spawner.last_update);

        if (elapsed_time < std::chrono::milliseconds(static_cast<long>(spawner.cooldown * 1000)))
            return;

        spawner.last_update = now;

        float count = 0.0f;

        std::for_each(ec.get_entities().begin(), ec.get_entities().end(), [&](auto e) {
            if (ec.get_entity_component<game::components::enemy>(e)) {
                ++count;
            }
        });
        if (count - 1 >= spawner.max_enemies)
            return;

        // spawn_enemy(ec, 100, 10, position, spawner.game);
        // spawn_enemy_chaser(ec, 0, 0.1f, position, spawner.game);
        // spawn_enemy_shooter(ec, 1.0f, position, spawner.game);
    }
    /**
        * @brief Move the enemy

        * This function is used to move the enemy entity with the given loop movement and position.
     */
    void move_enemy_loop(game::components::enemy_loop_movement &loop_mvt, ecs::components::position &position)
    {
        float delta_seconds = 1.0f / 60.0f;

        position.x += loop_mvt.speed * delta_seconds;

        if (loop_mvt.radius != 0.0f && loop_mvt.angular_speed != 0.0f) {
            loop_mvt.angle += loop_mvt.angular_speed * delta_seconds;
            position.y = loop_mvt.min_y + loop_mvt.radius * std::sin(loop_mvt.angle);
        } else {
            position.y += loop_mvt.speed * delta_seconds;

            if (position.y < loop_mvt.min_y) {
                position.y = loop_mvt.min_y;
                loop_mvt.speed = std::fabs(loop_mvt.speed);
            } else if (position.y > loop_mvt.max_y) {
                position.y = loop_mvt.max_y;
                loop_mvt.speed = -std::fabs(loop_mvt.speed);
            }
        }

        if (position.x < loop_mvt.min_x)
            position.x = loop_mvt.max_x;
        if (position.x > loop_mvt.max_x)
            position.x = loop_mvt.min_x;
    }
}
