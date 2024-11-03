module;

#include <nlohmann/json.hpp>
export module game:scenes.game;
import :game;
import :systems.map_editor;
import :components.map_editor;
import :components.spawn_waves;

import std;
import ecs;
import utils;

export namespace game::scenes {
    struct game_scene : public ecs::scene {
        constexpr game_scene(game &game) noexcept
            : ecs::scene(game), _game(game)
        {
            // emplace the spawn_waves component to enable the spawning of waves
            _game.emplace_component<components::spawn_waves>(_game, _game, "waves.json", 1);
        }

        protected:
            void create_entities() noexcept override
            {
                _entities.push_back(create_player());

                // _entities.push_back(spawn_enemy_chaser(_entities.back(), {500.0f, 500.0f}));
                // _entities.push_back(spawn_enemy({100.0f, 100.0f}));
                // init_waves("waves.json", 1);
                // _entities.push_back(spawn_enemy_spawner({300.0f, 300.0f}));
                // _entities.push_back(spawn_enemy_shooter({400.0f, 400.0f}));
            }

        private:
            game &_game;

            void init_waves(std::string_view path, int wave) noexcept
            {
                std::ifstream file(path.data());
                if (!file.is_open())
                    return;
                json waves;
                file >> waves;
                for (auto &wave : waves) {
                    std::vector<std::pair<std::string, ecs::abstractions::vector<float>>> mobs;
                    for (auto &mob : wave)
                    {
                        ecs::components::position position{0.0f, 0.0f};
                        position.x = mob["x"];
                        position.y = mob["y"];
                        _entities.push_back(spawn_enemy_shooter(position));
                        std::cout << "Spawned enemy at " << position.x << ", " << position.y << std::endl;

                        // if (mob["type"] == "enemy") {
                        //     spawn_enemy({mob["x"], mob["y"]});
                        // }
                        // else if (mob["type"] == "enemy_chaser") {
                        //     spawn_enemy_chaser(_entities.back(), {mob["x"], mob["y"]});
                        // }
                    }
                }
            }

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

            ecs::entity spawn_enemy(ecs::components::position position) noexcept
            {
                auto e = _game.create_entity();
                _game.add_component(e, position);
                _game.add_component(e, enemy{1, 100, std::chrono::steady_clock::now()});
                _game.add_component(e, health{1, _game});
                _game.add_component(e, ecs::components::engine::hitbox{rectangle<float>{position.x, position.y, 34.0f, 36.0f}});
                _game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
                _game.add_component(e, enemy_loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 1.0f, 0.0f, 100.0f, 2.0f, _game});
                _game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            "Enemy", _game.asset_manager.get("arial"), 12)},
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(
                            dynamic_cast<const ecs::abstractions::gui::texture &>(_game.asset_manager.get("enemy")), {8, 1}, 10ms)}
                    })
                });
                std::cout << "Spawned enemy at " << position.x << ", " << position.y << std::endl;
                return e;
            }

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

            ecs::entity spawn_enemy_spawner(ecs::components::position position) noexcept
            {
                auto e = _game.create_entity();
                _game.add_component(e, position);
                _game.emplace_component<enemy>(e, 0, 500, std::chrono::steady_clock::now());
                _game.emplace_component<health>(e, 3, _game);
                _game.emplace_component<enemy_spawner>(e, 2s, 5.f, _game);
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
