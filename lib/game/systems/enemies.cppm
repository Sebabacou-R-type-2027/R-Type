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

    void move_enemy_shooter(ecs::entity_container &ec, game::components::enemy_shooter &shooter, ecs::components::position& position)
    {
        const auto now = std::chrono::steady_clock::now();
        if (now - shooter.last_update < shooter.cooldown)
            return;

        auto windowSize = ec.get_entity_component<ecs::components::gui::window>(shooter.game)->get().window->getSize();
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(shooter.game);
        shooter.last_update = now;

        float speed = shooter._speed;

        printf("Shooter position: %f, %f\n", position.x, position.y);
        if (shooter.moving_up) {
            position.y -= speed;
            if (position.y <= 0) {
                shooter.moving_up = false;
            }
        } else {
            position.y += speed;
            if (position.y >= windowSize.y - 50) {
                shooter.moving_up = true;
            }
        }
    }

    void handle_enemy_spawner(ecs::entity e, ecs::entity_container &ec, components::enemy_spawner &spawner, ecs::components::position& position)
    {
        const auto now = std::chrono::steady_clock::now();
        if (now - spawner.last_update < spawner.cooldown)
            return;

        spawner.last_update = now;

        auto entities = ec.get_entities();
        std::size_t count = std::count_if(entities.begin(), entities.end(), [spawner = e, &ec](auto e){
            auto owned = ec.get_entity_component<components::enemy_ownership>(e).transform([spawner](components::enemy_ownership &enemy){
                return enemy.owner == spawner;
            });
            return owned.has_value() && owned.value();
        });

        if (count >= spawner.max_enemies)
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
