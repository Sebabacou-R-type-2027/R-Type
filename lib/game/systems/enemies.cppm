export module game:systems.enemies;
import :components.enemies;
import :components.projectiles;
import :components.stats;
import :systems.projectiles;

import std;
import ecs;
import utils;

using namespace std::chrono_literals;

export namespace game::systems {

    /**
        * @brief Move the enemy chaser

        * This function is used to move the enemy chaser entity with the given position and target.

        * @param ec The entity container
        * @param chaser The enemy chaser component
        * @param position The position of the enemy chaser
    */
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
        auto windowSize = ec.get_entity_component<ecs::components::gui::display>(shooter.game)->get().window->get_size();
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

        for (std::size_t i = 0; i < spawner.max_enemies; ++i) {
            const ecs::components::gui::display &display =
                *ec.get_entity_component<const ecs::components::gui::display>(spawner.game);
            const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(spawner.game);
            auto enemy = ec.create_entity();
            ec.add_component(enemy, ecs::components::position{position.x + i * 20.0f, position.y});
            ec.add_component(enemy, ecs::components::engine::velocity{-10.0f, 0.0f});
            ec.add_component(enemy, components::health{1, spawner.game});
            ec.add_component(enemy, ecs::components::engine::hitbox{ecs::abstractions::rectangle<float>{position.x, position.y, 34.0f, 36.0f}});
            ec.add_component(enemy, components::enemy_loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 1.0f, 0.0f, 100.0f, 2.0f});
            ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{spawner.game,
                std::container<ecs::components::gui::drawable::elements_container>::make({
                    {static_cast<ecs::entity>(spawner.game), display.factory->make_element(
                        "Enemy", asset_manager.get("arial"), 12)},
                    {static_cast<ecs::entity>(spawner.game), display.factory->make_element(
                        dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("enemy")), {8, 1}, 10ms)}
                })
            });
        }
    }

    /**
        * @brief Launch a projectile

        * This function is used to launch a projectile from the given entity.

        * @param e The entity that launches the projectile
        * @param ec The entity container
        * @param launcher The projectile launcher component
        * @param position The position of the entity
     */
    void pattern_1(ecs::entity e, ecs::entity_container &ec, components::boss &boss, ecs::components::position &position)
    {
        auto windowSize = ec.get_entity_component<ecs::components::gui::display>(boss.game)->get().window->get_size();
        position.y += 0.5f;

        if (position.y > windowSize.y - 200.0f || position.y < 200.0f)
            boss.moving_right = !boss.moving_right;

        if (boss.moving_right)
            position.y += boss.speed;
        else
            position.y -= boss.speed;

        if (std::chrono::steady_clock::now() - boss.last_attack > 500ms) {
            boss.last_attack = std::chrono::steady_clock::now();
            launch_projectile(e, ec, boss.launcher, position);
        }
    }

    /**
        * @brief Launch many projectiles

        * This function is used to launch many projectiles from the given entity.

        * @param e The entity that launches the projectiles
        * @param ec The entity container
        * @param launcher The projectile launcher component
        * @param position The position of the entity
     */
    void pattern_2(ecs::entity e, ecs::entity_container &ec, components::boss &boss, ecs::components::position &position)
    {
        auto windowSize = ec.get_entity_component<ecs::components::gui::display>(boss.game)->get().window->get_size();
        position.y += 0.5f;
        if (position.y > windowSize.y - 200.0f || position.y < 200.0f)
            boss.moving_right = !boss.moving_right;

        if (boss.moving_right)
            position.y += boss.speed * 2;
        else
            position.y -= boss.speed * 2;

        if (std::chrono::steady_clock::now() - boss.last_attack > 1.5s) {
            boss.last_attack = std::chrono::steady_clock::now();
            launch_many_projectiles(e, ec, boss.launcher, position);
            launch_cross_projectiles(e, ec, boss.launcher_cross, position);
        }
    }

    /**
        * @brief Launch cross projectiles

        * This function is used to launch cross projectiles from the given entity.

        * @param e The entity that launches the projectiles
        * @param ec The entity container
        * @param launcher The projectile launcher component
        * @param position The position of the entity
     */
    void pattern_3(ecs::entity e, ecs::entity_container &ec, components::boss &boss, ecs::components::position &position)
    {
        auto windowSize = ec.get_entity_component<ecs::components::gui::display>(boss.game)->get().window->get_size();

        position.x += boss.moving_right ? boss.speed * 1.5f : -boss.speed * 1.5f;
        position.y += std::sin(boss.angle) * 1.0f;
        boss.angle += 0.1f;

        if (position.x > windowSize.x - 200.0f || position.x < 200.0f)
            boss.moving_right = !boss.moving_right;
        if (std::chrono::steady_clock::now() - boss.last_attack > 1s) {
            boss.last_attack = std::chrono::steady_clock::now();
            launch_spiral_projectiles(e, ec, boss.launcher_spiral, position);
        }

        auto target_position = ec.get_entity_component<ecs::components::position>(boss.target);
        if (target_position && std::abs(position.x - target_position->get().x) < 50.0f) {
            position.x += (target_position->get().x > position.x ? 1 : -1) * boss.speed;
            position.y += (target_position->get().y > position.y ? 1 : -1) * boss.speed;
        }
    }

    /**
        * @brief Handle the boss pattern

        * This function is used to handle the boss entity. It will move the boss entity towards the target entity.

        * @param e The entity of the boss
        * @param ec The entity container
        * @param boss The boss component
        * @param position The position of the boss
     */
    void handle_boss_pattern(ecs::entity e, ecs::entity_container &ec, components::boss &boss, ecs::components::position& position)
    {
        auto life = ec.get_entity_component<components::health>(e);

        if (life->get().value >= 250) {
            pattern_1(e, ec, boss, position);
        }
        if (life->get().value < 200 && life->get().value >= 100) {
            pattern_2(e, ec, boss, position);
        }
        if (life->get().value < 100 && life->get().value > 0) {
            pattern_3(e, ec, boss, position);
        }

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
    }
}
