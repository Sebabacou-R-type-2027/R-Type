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
    /**
        * @brief Create an enemy

        * This function is used to create an enemy entity with the given health, damage and position.
     */
    void spawn_enemy(ecs::entity_container &ec, int health, int damage, const ecs::components::position& position, ecs::entity game)
    {
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(game);
        auto enemy = ec.create_entity();
        ec.add_component(enemy, components::enemy{health, damage, std::chrono::steady_clock::now()});
        ec.add_component(enemy, ecs::components::position{position.x, position.y});
        ec.add_component(enemy, ecs::components::engine::velocity{10.0f, 0.0f});
        ec.add_component(enemy, game::components::enemy_loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 1.0f, 0.0f, 100.0f, 2.0f});
        ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Enemy", asset_manager.get_font("arial"), 12), "arial")},
                {game, std::make_unique<ecs::components::gui::animation>(
                    asset_manager.get_texture("enemy"), 1, 8, 10ms, "enemy")}
            })
        });
    }

    /**
        * @brief Create an enemy chaser

        * This function is used to create an enemy chaser entity with the given target, speed and position.
     */
    void spawn_enemy_chaser(ecs::entity_container &ec, ecs::entity target, float speed, const ecs::components::position& position, ecs::entity game)
    {
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(game);
        auto enemy = ec.create_entity();
        ec.add_component(enemy, components::enemy{100, 10, std::chrono::steady_clock::now()});
        ec.add_component(enemy, ecs::components::position{position.x, position.y});
        ec.add_component(enemy, ecs::components::engine::velocity{10.0f, 0.0f});
        ec.add_component(enemy, game::components::enemy_chaser{target, speed});
        ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Chaser", asset_manager.get_font("arial"), 12), "arial")},
                {game, std::make_unique<ecs::components::gui::animation>
                    (asset_manager.get_texture("enemy_chaser"), 1, 3, 10ms, "enemy_chaser")
                }
            })
        });
    }

    /**
        * @brief Create an enemy shooter

        * This function is used to create an enemy shooter entity with the given cooldown and position.
     */
    void spawn_enemy_shooter(ecs::entity_container &ec, float cooldown, const ecs::components::position& position, ecs::entity game)
    {
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(game);
        auto enemy = ec.create_entity();
        ec.add_component(enemy, components::enemy{100, 10, std::chrono::steady_clock::now()});
        ec.add_component(enemy, ecs::components::position{position.x, position.y});
        ec.add_component(enemy, ecs::components::engine::velocity{10.0f, 0.0f});
        ec.add_component(enemy, game::components::enemy_shooter{cooldown, std::chrono::steady_clock::now(), game});
        ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Shooter", asset_manager.get_font("arial"), 12), "arial")},
                {game, std::make_unique<ecs::components::gui::animation>
                    (asset_manager.get_texture("enemy_shooter"), 1, 3, 10ms, "enemy_shooter")
                }
            })
        });
    }

    /**
        * @brief Create an enemy spawner

        * This function is used to create an enemy spawner entity with the given cooldown, max enemies and position.
     */
    void spawn_enemy_spawner(ecs::entity_container &ec, float cooldown, int max_enemies, const ecs::components::position& position, ecs::entity game)
    {
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(game);
        auto enemy = ec.create_entity();
        ec.add_component(enemy, components::enemy{100, 10, std::chrono::steady_clock::now()});
        ec.add_component(enemy, ecs::components::position{position.x, position.y});
        ec.add_component(enemy, ecs::components::engine::velocity{0.0f, 0.0f});
        ec.add_component(enemy, game::components::enemy_spawner{cooldown, max_enemies, std::chrono::steady_clock::now(), game});
                ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Spawner", asset_manager.get_font("arial"), 12), "arial")},
                {game, std::make_unique<ecs::components::gui::animation>
                    (asset_manager.get_texture("enemy_spawner"), 1, 5, 10ms, "enemy_spawner")
                }
            })
        });
    }

    void move_enemy_chaser(ecs::entity_container &ec, game::components::enemy_chaser &chaser, ecs::components::position& position, std::chrono::milliseconds dt)
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

        position.x += vx * dt.count();
        position.y += vy * dt.count();
    }

    void move_enemy_shooter(ecs::entity_container &ec, game::components::enemy_shooter &shooter, ecs::components::position& position, std::chrono::milliseconds dt)
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

    void handle_enemy_spawner(ecs::entity_container &ec, game::components::enemy_spawner &spawner, ecs::components::position& position, std::chrono::milliseconds dt)
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

        spawn_enemy(ec, 100, 10, position, spawner.game);
        // spawn_enemy_chaser(ec, 0, 0.1f, position, spawner.game);
        // spawn_enemy_shooter(ec, 1.0f, position, spawner.game);
    }
    /**
        * @brief Move the enemy

        * This function is used to move the enemy entity with the given loop movement and position.
     */
    void move_enemy_loop(game::components::enemy_loop_movement &loop_mvt, ecs::components::position &position, std::chrono::milliseconds dt, ecs::entity game)
    {
        float delta_seconds = std::chrono::duration<float>(dt).count();

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
