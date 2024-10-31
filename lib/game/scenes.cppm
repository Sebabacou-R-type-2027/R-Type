#if __cpp_lib_modules < 202207L
module;

#include <chrono>
#include <memory>
#endif
export module game:scenes;
import :game;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

export namespace game::scenes {
    struct game_scene : public ecs::scene {
        constexpr game_scene(game &game) noexcept
            : ecs::scene(game), _game(game)
        {}

        protected:
            void create_entities() noexcept override
            {
                _entities.push_back(create_player());
                _entities.push_back(create_fire_button(*_game.get_entity_component<projectile_launcher>(_entities.back())));
                _entities.push_back(spawn_enemy({100.0f, 100.0f}));
                _entities.push_back(spawn_enemy_chaser(_entities.back(), {200.0f, 200.0f}));
                _entities.push_back(spawn_enemy_spawner({300.0f, 300.0f}));
                _entities.push_back(spawn_enemy_shooter({400.0f, 400.0f}));
            }

        private:
            game &_game;

            ecs::entity create_player() noexcept
            {
                auto player = _ec.create_entity();
                _game.emplace_component<ecs::components::position>(player, 50.0f, 50.0f);
                _game.emplace_component<ecs::components::engine::velocity>(player, 0.0f, 0.0f);
                _game.emplace_component<ecs::components::engine::controllable>(player, _game, true, 10.0f);
                _game.add_component<projectile_launcher>(player, {1s, std::chrono::steady_clock::now(), _game});
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

            ecs::entity spawn_enemy(ecs::components::position position) noexcept
            {
                auto e = _game.create_entity();
                _game.add_component(e, position);
                _game.add_component(e, enemy{1, 1, std::chrono::steady_clock::now()});
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

            ecs::entity spawn_enemy_chaser(ecs::entity target, ecs::components::position position) noexcept
            {
                auto e = _game.create_entity();
                _game.add_component(e, position);
                _game.add_component(e, enemy{100, 10, std::chrono::steady_clock::now()});
                _game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
                _game.add_component(e, enemy_chaser{target, 10.0f});
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
                _game.emplace_component<enemy>(e, 100, 10, std::chrono::steady_clock::now());
                _game.emplace_component<enemy_spawner>(e, 2s, 5ul, _game);
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
                _game.add_component(e, enemy{100, 10, std::chrono::steady_clock::now()});
                _game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
                _game.add_component(e, enemy_shooter{2s, _game});
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

    struct menu : public ecs::scene {
        constexpr menu(game &game) noexcept
            : ecs::scene(game), _game(game)
        {}

        protected:
            void create_entities() noexcept override
            {
                auto button = _game.create_entity();
                _game.emplace_component<ecs::components::position>(button, 960, 540);
                _game.emplace_component<components::button>(button, _game, ecs::abstractions::vector<float>(100, 50),
                        [&game = _game](){ game.begin_scene(std::make_unique<game_scene>(game)); });
                auto label = _game.display.factory->make_element(
                    "Play", _game.asset_manager.get("arial"), 48);
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
                _entities.push_back(button);
            }

        private:
            game &_game;
    };
}
