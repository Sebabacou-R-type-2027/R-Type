export module game:scenes.game;
import :game;

import std;
import ecs;
import utils;

export namespace game::scenes {
    struct game_scene : public ecs::scene {
        constexpr game_scene(game &game) noexcept
            : ecs::scene(game), _game(game)
        {}

        protected:
            /**
                * @brief Create the entities

                * This function is used to create the entities of the scene.

                * @return The entities of the scene
             */
            void create_entities() noexcept override
            {
                _entities.push_back(create_player());
                // _entities.push_back(create_fire_button(*_game.get_entity_component<projectile_launcher>(_entities.back())));
                std::ranges::for_each(_entities, [this](ecs::entity e) {
                    if (auto launcher = _game.get_entity_component<components::score>(e)) {
                        _entities.push_back(spawn_boss(e, {1000.0f, 500.0f}));
                        _entities.push_back(spawn_enemy_chaser(e, {500.0f, 500.0f}));
                    }
                });
                // _entities.push_back(spawn_enemy({100.0f, 100.0f}));
                // _entities.push_back(spawn_enemy_spawner({300.0f, 300.0f}));
                // _entities.push_back(spawn_enemy_shooter({400.0f, 400.0f}));
            }

            private:
                game &_game;

            /**
                * @brief Create the player

                * This function is used to create the player entity.

                * @return The player entity
             */
            ecs::entity create_player() noexcept
            {
                auto player = _ec.create_entity();
                _game.emplace_component<ecs::components::position>(player, 50.0f, 50.0f);
                _game.emplace_component<ecs::components::engine::velocity>(player, 0.0f, 0.0f);
                _game.emplace_component<ecs::components::engine::controllable>(player, _game, true, 10.0f);
                _game.emplace_component<ecs::components::engine::hitbox>(player, rectangle<float>{50.0f, 50.0f, 34.0f, 36.0f});
                _game.emplace_component<components::score>(player, 0, _game);
                _game.emplace_component<components::health>(player, 1, _game);
                _game.add_component(player, components::projectile_launcher_ownership{100ms, std::chrono::steady_clock::now(), _game, false});
                auto label = _game.display.factory->make_element("Player", _game.asset_manager.get("arial"), 12);
                label->set_origin({label->bounds(true).width / 2, label->bounds(true).height / 2});
                _game.emplace_component<ecs::components::gui::drawable>(player, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), std::move(label)},
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            dynamic_cast<const ecs::abstractions::gui::texture &>(_game.asset_manager.get("ship")),
                            {5, 1})}
                    })
                });
                return player;
            }

            /**
                * @brief Create the fire button

                * This function is used to create the fire button entity.

                * @param launcher The projectile launcher component
                * @return The fire button entity
             */
            ecs::entity create_fire_button(components::projectile_launcher &launcher) noexcept
            {
                auto button = _game.create_entity();
                _game.emplace_component<ecs::components::position>(button, 230.f, 875.f);
                _game.emplace_component<components::button>(button, _game, ecs::abstractions::vector<float>(100, 50),
                        [&launcher](){ launcher.last_shot = std::chrono::steady_clock::time_point(0s); });
                auto label = _game.display.factory->make_element(
                    "Fire", _game.asset_manager.get("arial"), 48);
                label->set_origin({label->bounds(true).width / 2, label->bounds(true).height / 2});
                label->set_repositioning(ecs::abstractions::gui::drawable_element::reposition_center);
                auto background = _game.display.factory->make_element({100, 50},
                    ecs::abstractions::gui::color::cyan);
                background->set_origin({background->bounds(true).width / 2, background->bounds(true).height / 2});
                background->set_repositioning(ecs::abstractions::gui::drawable_element::reposition_center);
                _game.emplace_component<ecs::components::gui::drawable>(button, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), std::move(background)},
                        {static_cast<ecs::entity>(_game), std::move(label)}
                    })
                });
                return button;
            }

            /**
                * @brief Spawn an enemy

                * This function is used to spawn an enemy entity.

                * @param position The position of the enemy
                * @return The enemy entity
             */
            ecs::entity spawn_enemy(ecs::components::position position) noexcept
            {
                auto e = _game.create_entity();
                _game.add_component(e, position);
                _game.add_component(e, enemy{1, 100, std::chrono::steady_clock::now()});
                _game.add_component(e, health{1, _game});
                _game.add_component(e, ecs::components::engine::hitbox{rectangle<float>{position.x, position.y, 34.0f, 36.0f}});
                _game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
                _game.add_component(e, enemy_loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 1.0f, 0.0f, 100.0f, 2.0f});
                _game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            "Enemy", _game.asset_manager.get("arial"), 12)},
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            dynamic_cast<const ecs::abstractions::gui::texture &>(_game.asset_manager.get("enemy")), {8, 1}, 10ms)}
                    })
                });
                return e;
            }

            /**
                * @brief Spawn a boss

                * This function is used to spawn a boss entity.

                * @param target The target entity
                * @param position The position of the boss
                * @return The boss entity
             */
            ecs::entity spawn_boss(ecs::entity target, ecs::components::position position) noexcept
            {
                auto e = _game.create_entity();
                _game.add_component(e, position);
                _game.add_component(e, enemy{1, 1000, std::chrono::steady_clock::now()});
                _game.add_component(e, health{300, _game});
                _game.add_component(e, boss{target, _game});
                _game.add_component(e, ecs::components::engine::hitbox{rectangle<float>{position.x, position.y, 160.0f, 212.0f}});
                _game.add_component(e, ecs::components::engine::velocity{0.0f, 0.0f});
                _game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            "Boss", _game.asset_manager.get("arial"), 12)},
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            dynamic_cast<const ecs::abstractions::gui::texture &>(_game.asset_manager.get("boss-phase")), {4, 1}, 50ms)}
                    })
                });
                return e;
            }

            /**
                * @brief Spawn an enemy chaser

                * This function is used to spawn an enemy chaser entity.

                * @param target The target entity
                * @param position The position of the enemy chaser
                * @return The enemy chaser entity
             */
            ecs::entity spawn_enemy_chaser(ecs::entity target, ecs::components::position position) noexcept
            {
                auto e = _game.create_entity();
                _game.add_component(e, position);
                _game.add_component(e, enemy{1, 300, std::chrono::steady_clock::now()});
                _game.add_component(e, health{1, _game});
                _game.add_component(e, ecs::components::engine::hitbox{rectangle<float>{position.x, position.y, 33.2f, 34.0f}});
                _game.add_component(e, ecs::components::engine::velocity{0.0f, 0.0f});
                _game.add_component(e, enemy_chaser{target, 5.0f});
                _game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            "Chaser", _game.asset_manager.get("arial"), 12)},
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            dynamic_cast<const ecs::abstractions::gui::texture &>(_game.asset_manager.get("enemy_chaser")), {3, 1}, 10ms)}
                    })
                });
                return e;
            }

            /**
                * @brief Spawn an enemy spawner

                * This function is used to spawn an enemy spawner entity.

                * @param position The position of the enemy spawner
                * @return The enemy spawner entity
             */
            ecs::entity spawn_enemy_spawner(ecs::components::position position) noexcept
            {
                auto e = _game.create_entity();
                _game.add_component(e, position);
                _game.emplace_component<enemy>(e, 0, 500, std::chrono::steady_clock::now());
                _game.emplace_component<health>(e, 3, _game);
                _game.emplace_component<enemy_spawner>(e, 2s, 5ul, _game);
                _game.emplace_component<ecs::components::engine::hitbox>(e, rectangle<float>{position.x, position.y, 65.0f, 66.0f});
                _game.emplace_component<ecs::components::engine::velocity>(e);
                _game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            "Spawner", _game.asset_manager.get("arial"), 12)},
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            dynamic_cast<const ecs::abstractions::gui::texture &>(_game.asset_manager.get("enemy_spawner")), {5, 1}, 10ms)}
                    })
                });
                return e;
            }

            /**
                * @brief Spawn an enemy shooter

                * This function is used to spawn an enemy shooter entity.

                * @param position The position of the enemy shooter
                * @return The enemy shooter entity
             */
            ecs::entity spawn_enemy_shooter(ecs::components::position position) noexcept
            {
                auto e = _game.create_entity();
                _game.add_component(e, position);
                _game.add_component(e, enemy{1, 250, std::chrono::steady_clock::now()});
                _game.add_component(e, health{1, _game});
                _game.add_component(e, projectile_launcher{-1.0, 1s, std::chrono::steady_clock::now(), _game});
                _game.add_component(e, ecs::components::engine::hitbox{rectangle<float>{position.x, position.y, 65.0f, 50.0f}});
                _game.add_component(e, ecs::components::engine::velocity{0.0f, 0.0f});
                _game.add_component(e, enemy_shooter{true, 10.0f, _game});
                _game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            "Shooter", _game.asset_manager.get("arial"), 12)},
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            dynamic_cast<const ecs::abstractions::gui::texture &>(_game.asset_manager.get("enemy_shooter")), {3, 1}, 10ms)}
                    })
                });
                return e;
            }
    };
}
