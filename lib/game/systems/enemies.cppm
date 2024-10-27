module;

#if __cpp_lib_modules < 202207L
#include <chrono>
#endif
#include <SFML/Graphics.hpp>
#include <cmath>
export module game:systems.enemies;
import :components.enemies;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

export namespace game::systems {
    /**
        * @brief Create an enemy

        * This function is used to create an enemy entity with the given health, damage and position.
     */
    void spawn_enemy(ecs::entity_container &ec, int health, int damage, const ecs::components::position& position, ecs::entity game)
    {
        auto enemy = ec.create_entity();
        ec.add_component(enemy, components::enemies{health, damage, std::chrono::steady_clock::now()});
        ec.add_component(enemy, ecs::components::position{position.x, position.y});
        ec.add_component(enemy, ecs::components::engine::velocity{10.0f, 0.0f});
        ec.add_component(enemy, ecs::components::enemies::loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 0.0f, 100.0f, 2.0f});
        ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{game,
            std::initializer_list<ecs::components::gui::drawable::elements_container::value_type>{
                {game, {
                    std::make_shared<sf::Text>("Enemy",
                        ec.get_entity_component<ecs::components::gui::asset_manager>(0)->get().get_font("arial"), 12),
                        "arial"
                    }
                }
            }
        });
    }

    /**
        * @brief Create an enemy chaser

        * This function is used to create an enemy chaser entity with the given target, speed and position.
     */
    void spawn_enemy_chaser(ecs::entity_container &ec, ecs::entity target, float speed, const ecs::components::position& position, ecs::entity game)
    {
        auto enemy = ec.create_entity();
        ec.add_component(enemy, components::enemies{100, 10, std::chrono::steady_clock::now()});
        ec.add_component(enemy, ecs::components::position{position.x, position.y});
        ec.add_component(enemy, ecs::components::engine::velocity{10.0f, 0.0f});
        ec.add_component(enemy, ecs::components::enemies::enemy_chaser{target, speed});
        ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{game,
            std::initializer_list<ecs::components::gui::drawable::elements_container::value_type>{
                {game, {
                    std::make_shared<sf::Text>("Enemy Chaser",
                        ec.get_entity_component<ecs::components::gui::asset_manager>(0)->get().get_font("arial"), 12),
                        "arial"
                    }
                }
            }
        });
    }

    /**
        * @brief Create an enemy shooter

        * This function is used to create an enemy shooter entity with the given cooldown and position.
     */
    void spawn_enemy_shooter(ecs::entity_container &ec, float cooldown, const ecs::components::position& position, ecs::entity game)
    {
        auto enemy = ec.create_entity();
        ec.add_component(enemy, components::enemies{100, 10, std::chrono::steady_clock::now()});
        ec.add_component(enemy, ecs::components::position{position.x, position.y});
        ec.add_component(enemy, ecs::components::engine::velocity{10.0f, 0.0f});
        ec.add_component(enemy, ecs::components::enemies::enemy_shooter{cooldown, std::chrono::milliseconds(0)});
        ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{game,
            std::initializer_list<ecs::components::gui::drawable::elements_container::value_type>{
                {game, {
                    std::make_shared<sf::Text>("Enemy Shooter",
                        ec.get_entity_component<ecs::components::gui::asset_manager>(0)->get().get_font("arial"), 12),
                        "arial"
                    }
                }
            }
        });
    }

    /**
        * @brief Create an enemy spawner

        * This function is used to create an enemy spawner entity with the given cooldown, max enemies and position.
     */
    void spawn_enemy_spawner(ecs::entity_container &ec, float cooldown, int max_enemies, const ecs::components::position& position, ecs::entity game)
    {
        auto enemy = ec.create_entity();
        ec.add_component(enemy, components::enemies{100, 10, std::chrono::steady_clock::now()});
        ec.add_component(enemy, ecs::components::position{position.x, position.y});
        ec.add_component(enemy, ecs::components::engine::velocity{0.0f, 0.0f});
        ec.add_component(enemy, ecs::components::enemies::enemy_spawner{cooldown, max_enemies, std::chrono::milliseconds(0)});
        ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{game,
            std::initializer_list<ecs::components::gui::drawable::elements_container::value_type>{
                {game, {
                    std::make_shared<sf::Text>("Enemy Spawner",
                        ec.get_entity_component<ecs::components::gui::asset_manager>(0)->get().get_font("arial"), 12),
                        "arial"
                    }
                }
            }
        });
    }

    void move_enemy_chaser(ecs::entity_container &ec, ecs::components::enemies::enemy_chaser &chaser, ecs::components::positions& position, std::chrono::milliseconds dt)
    {
        if (!ecs::entity_exists(chaser._target))
            return;
        auto target_position = ecs::get_entity_component<ecs::components::positions>(chaser._target);
        auto target_velocity = ecs::get_entity_component<ecs::components::engine::velocity>(chaser._target);

        float dx = target_position.x - position.x;
        float dy = target_position.y - position.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < 1.0f)
            return;

        float vx = dx / distance * chaser._speed;
        float vy = dy / distance * chaser._speed;

        position.x += vx * dt.count();
        position.y += vy * dt.count();
    }

    void move_enemy_shooter(ecs::entity_container &ec, ecs::components::enemies::enemy_shooter &shooter, ecs::components::positions& position, std::chrono::milliseconds dt)
    {
        auto now = std::chrono::steady_clock::now();
        if (now - shooter.last_update < std::chrono::milliseconds(static_cast<long>(shooter.cooldown * 1000)))
            return;

        shooter.last_update = now;

        auto player = ecs::get_entity_component<ecs::components::positions>(0);
        auto player_velocity = ecs::get_entity_component<ecs::components::engine::velocity>(0);

        float dx = player->x - position.x;
        float dy = player->y - position.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < 1.0f)
            return;

        float vx = dx / distance * 10.0f;
        float vy = dy / distance * 10.0f;

        auto projectile = ec.create_entity();
        ec.add_component(projectile, components::projectile{10, now, 3s});
        ec.add_component(projectile, ecs::components::position{position.x, position.y});
        ec.add_component(projectile, ecs::components::engine::velocity{vx, vy});
        ec.emplace_component<ecs::components::gui::drawable>(projectile, ecs::components::gui::drawable{shooter.game,
            std::initializer_list<ecs::components::gui::drawable::elements_container::value_type>{
                {shooter.game, {
                    std::make_shared<sf::Text>("Pew",
                        ec.get_entity_component<ecs::components::gui::asset_manager>(shooter.game)->get().get_font("arial"), 12),
                        "arial"
                    }
                }
            }
        });
    }

    void handle_enemy_spawner(ecs::entity_container &ec, ecs::components::enemies::enemy_spawner &spawner, ecs::components::positions& position, std::chrono::milliseconds dt)
    {
        auto now = std::chrono::steady_clock::now();
        if (now - spawner.last_update < std::chrono::milliseconds(static_cast<long>(spawner.cooldown * 1000)))
            return;

        spawner.last_update = now;

        if (ec.get_entities_count() - 1 >= spawner.max_enemies)
            return;

        spawn_enemy(ec, 100, 10, position, spawner.game);
        // spawn_enemy_chaser(ec, 0, 0.1f, position, spawner.game);
        // spawn_enemy_shooter(ec, 1.0f, position, spawner.game);
    }
    /**
        * @brief Move the enemy

        * This function is used to move the enemy entity with the given loop movement and position.
     */
    void move_enemy_loop(ecs::components::enemies::loop_movement &loop_mvt, ecs::components::positions& position, std::chrono::milliseconds dt, ecs::entity game)
    {
        position.x += loop_mvt.speed * dt.count();

        if (loop_mvt.radius != 0.0f && loop_mvt.angular_speed != 0.0f) {
            loop_mvt.angle += loop_mvt.angular_speed * deltaTime;

            position.y = loop_mvt.min_y + loop_mvt.radius * sin(loop_mvt.angle);
        } else {
            position.y += loop_mvt.speed * deltaTime;

            if (position.y < loop_mvt.min_y) {
                position.y = loop_mvt.min_y;
                loop_mvt.speed = fabs(loop_mvt.speed);
            } else if (position.y > loop_mvt.max_y) {
                position.y = loop_mvt.max_y;
                loop_mvt.speed = -fabs(loop_mvt.speed);
            }
        }

        if (position.x < loop_mvt.min_x)
            position.x = loop_mvt.max_x;
        if (position.x > loop_mvt.max_x)
            position.x = loop_mvt.min_x;
    }
}