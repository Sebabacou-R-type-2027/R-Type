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
                // _entities.push_back(create_fire_button(*_game.get_entity_component<projectile_launcher>(_entities.back())));
                _entities.push_back(spawn_enemy_chaser(_entities.back(), {500.0f, 500.0f}));
                _entities.push_back(spawn_enemy({100.0f, 100.0f}));
                // _entities.push_back(spawn_enemy_spawner({300.0f, 300.0f}));
                // _entities.push_back(spawn_enemy_shooter({400.0f, 400.0f}));
            }

        private:
            game &_game;

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

    struct menu : public ecs::scene {
        constexpr menu(game &game) noexcept
            : ecs::scene(game), _game(game)
        {}

        protected:
            void create_entities() noexcept override
            {
                ecs::abstractions::vector<float> window_size = _game.display.window->get_size();
                float spacing = 20.0f;
                float total_height = (50.0f + spacing) * 7;
                float start_y = (window_size.y - total_height) / 2.0f;
                int button_height;
                int fontSize = 24;
                ecs::abstractions::gui::color button_color = ecs::abstractions::gui::color(14, 94, 255, 255);

                create_centered_text("R-Type", 50.0f, button_color, 96);

                button_height = create_centered_button("Solo Campaign", start_y, fontSize, button_color, [&game = _game](){
                    game.begin_scene(std::make_unique<game_scene>(game));
                });
                start_y += button_height + spacing;

                button_height = create_centered_button("Multiplayer", start_y, fontSize, button_color, [&game = _game](){
                    game.begin_scene(std::make_unique<game_scene>(game));
                });
                start_y += button_height + spacing;

                button_height = create_centered_button("Customize Ship", start_y, fontSize, button_color, [&game = _game](){
                    game.begin_scene(std::make_unique<game_scene>(game));
                });
                start_y += button_height + spacing;

                button_height = create_centered_button("Settings", start_y, fontSize, button_color, [&game = _game](){
                    game.begin_scene(std::make_unique<game_scene>(game));
                });
                start_y += button_height + spacing;

                button_height = create_centered_button("Map Editor", start_y, fontSize, button_color, [&game = _game](){
                    game.begin_scene(std::make_unique<game_scene>(game));
                });
                start_y += button_height + spacing;

                button_height = create_centered_button("Game Options (ON/OFF Shader)", start_y, fontSize, button_color, [&game = _game](){
                    game.begin_scene(std::make_unique<game_scene>(game));
                });
                start_y += button_height + spacing;

                button_height = create_centered_button("Quit", start_y, fontSize, button_color, [&game = _game](){
                    game.display.window->close();
                });
            }

        private:
            game &_game;

            int create_centered_button(const std::string &label, float yPos, int fontSize, ecs::abstractions::gui::color button_color, std::function<void()> action) noexcept
            {
                ecs::abstractions::vector<float> window_size = _game.display.window->get_size();
                auto text = _game.display.factory->make_element(label, _game.asset_manager.get("arial"), fontSize);
                float text_top = text->bounds(true).y;
                float button_height = 50.0f;

                ecs::abstractions::vector<float> text_size = {text->bounds(true).width, text->bounds(true).height};
                create_text({(window_size.x - text_size.x) / 2, yPos + text_top}, label, ecs::abstractions::gui::color::white, fontSize);

                ecs::abstractions::vector<float> button_size = {text_size.x + 40, button_height};
                create_clickable_rectangle({(window_size.x - button_size.x) / 2, yPos}, button_size, button_color, action);

                return button_size.y;
            }

            void create_rectangle(ecs::abstractions::vector<float> position, ecs::abstractions::vector<float> size, ecs::abstractions::gui::color color) noexcept
            {
                auto rectangle = _game.create_entity();
                _game.emplace_component<ecs::components::position>(rectangle, position.x, position.y);
                _game.emplace_component<ecs::components::gui::drawable>(rectangle, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(size, color)}
                    })
                });
                _entities.push_back(rectangle);
            }

            void create_text(ecs::abstractions::vector<float> position, std::string text, ecs::abstractions::gui::color color, int size = 24) noexcept
            {
                auto text_entity = _game.create_entity();
                _game.emplace_component<ecs::components::position>(text_entity, position.x, position.y);
                _game.emplace_component<ecs::components::gui::drawable>(text_entity, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(text, _game.asset_manager.get("arial"), size, color)}
                    })
                });
                _entities.push_back(text_entity);
            }

            void create_centered_text(std::string label, float yPos, ecs::abstractions::gui::color color, int fontSize = 24) noexcept
            {
                ecs::abstractions::vector<float> window_size = _game.display.window->get_size();
                auto text = _game.display.factory->make_element(label, _game.asset_manager.get("arial"), fontSize);
                float text_top = text->bounds(true).y;
                float button_height = 50.0f;

                ecs::abstractions::vector<float> text_size = {text->bounds(true).width, text->bounds(true).height};
                create_text({(window_size.x - text_size.x) / 2, yPos + text_top}, label, color, fontSize);
            }

            void create_clickable_rectangle(ecs::abstractions::vector<float> position, ecs::abstractions::vector<float> size, ecs::abstractions::gui::color color, std::function<void()> action) noexcept
            {
                auto rectangle = _game.create_entity();
                _game.emplace_component<ecs::components::position>(rectangle, position.x, position.y);
                _game.emplace_component<ecs::components::gui::drawable>(rectangle, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(size, color)}
                    })
                });
                _game.emplace_component<components::button>(rectangle, _game, size, action);
                _entities.push_back(rectangle);
            }
    };
}
