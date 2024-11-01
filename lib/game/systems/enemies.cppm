#if __cpp_lib_modules < 202207L
module;

#include <cmath>
#include <chrono>
#endif
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
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 1.0f) {
            return;
        }

        float vx = dx / distance * chaser._speed;
        float vy = dy / distance * chaser._speed;

        position.x += vx;
        position.y += vy;
    }

    /**
        * @brief Move the enemy shooter

        * This function is used to move the enemy shooter entity with the given position and velocity.

        * @param ec The entity container
        * @param shooter The enemy shooter component
        * @param position The position of the enemy shooter
        * @param velocity The velocity of the enemy shooter
     */
    void move_enemy_shooter(ecs::entity_container &ec, game::components::enemy_shooter &shooter, ecs::components::position& position, ecs::components::engine::velocity &velocity)
    {
        auto windowSize = ec.get_entity_component<ecs::components::gui::window>(shooter.game)->get().window->getSize();
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(shooter.game);

        float speed = shooter.speed;

        if (shooter.moving_up) {
            velocity.y = -speed;
            if (position.y <= 0) {
                shooter.moving_up = false;
            }
        } else {
            velocity.y = speed;
            if (position.y >= windowSize.y - 50) {
                shooter.moving_up = true;
            }
        }
    }

    /**
        * @brief Handle the enemy spawner

        * This function is used to handle the enemy spawner entity. It will spawn an enemy if the cooldown has passed and the maximum number of enemies has not been reached.

        * @param e The entity of the enemy spawner
        * @param ec The entity container
        * @param spawner The enemy spawner component
        * @param position The position of the enemy spawner
     */
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
        
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(spawner.game);
        auto enemy = ec.create_entity();
        ec.add_component(enemy, ecs::components::position{position.x, position.y});
        ec.add_component(enemy, ecs::components::engine::velocity{-10.0f, 0.0f});
        ec.add_component(enemy, ecs::components::lifestate{});
        ec.add_component<enemy, components::health(e, 1, game);
        ec.add_component(enemy, ecs::components::engine::hitbox{34.0f, 36.0f, 0.0f, 0.0f});
        ec.add_component(enemy, components::enemy_loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 1.0f, 0.0f, 100.0f, 2.0f});
        ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{spawner.game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {static_cast<ecs::entity>(spawner.game), std::make_unique<ecs::components::gui::animation>(
                    asset_manager.get_texture("enemy"), 1, 8, 200ms, "enemy")
                }
            })
        });
    }
    /**
        * @brief Move the enemy

        * This function is used to move the enemy entity with the given loop movement and position.

        * @param loop_mvt The enemy loop movement component
        * @param position The position of the enemy
     */
    void move_enemy_loop(game::components::enemy_loop_movement &loop_mvt, ecs::components::position &position)
    {
        float delta_seconds = 0.05f;

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
