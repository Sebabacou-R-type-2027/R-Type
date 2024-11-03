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
            {
            }

        protected:
            void create_entities() noexcept override
            {
                auto network = _game.get_entity_component<components::network>(_game);

                for (int i = 0; i - 1 != network->get().client->number_of_players_; i++) {
                    _entities.push_back(create_player(100, 150 * (i + 1)));
                }
                // _entities.push_back(create_fire_button(*_game.get_entity_component<projectile_launcher>(_entities.back())));
                _entities.push_back(spawn_enemy_chaser(_entities.back(), {500.0f, 500.0f}));
                _entities.push_back(spawn_enemy({100.0f, 100.0f}));
                // _entities.push_back(spawn_enemy_spawner({300.0f, 300.0f}));
                // _entities.push_back(spawn_enemy_shooter({400.0f, 400.0f}));
            }

            private:
                game &_game;

            ecs::entity create_player(float x, float y) noexcept
            {
                auto player = _ec.create_entity();
                _game.emplace_component<ecs::components::position>(player, x, y);
                _game.emplace_component<ecs::components::engine::velocity>(player, 0.0f, 0.0f);
                _game.emplace_component<ecs::components::engine::controllable>(player, _game, true, 10.0f);
                _game.emplace_component<ecs::components::engine::hitbox>(player, rectangle<float>{x, x, 34.0f, 36.0f});
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

        struct multiplayer_menu : public ecs::scene {
        constexpr multiplayer_menu(game &game) noexcept
            : ecs::scene(game), _game(game)
        {}

        protected:
            void create_entities() noexcept
            {
                initUI();
            }

        private:
            game &_game;

            void initUI() {
                float width = 450.0f;
                float height = 600.0f;
                ecs::abstractions::gui::color fillColor = ecs::abstractions::gui::color(100, 100, 100, 255 * 0.7);
                ecs::abstractions::gui::color outlineColor = ecs::abstractions::gui::color::blue;
                float buttonWidth = 400.0f;
                float buttonHeight = 50.0f;

                // Input rectangle for Username
                auto username_input = _game.create_entity();
                _game.emplace_component<ecs::components::position>(username_input, (_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 140.0f);
                auto username_text = _game.display.factory->make_element("", _game.asset_manager.get("arial"), 24);
                auto &username = _game.emplace_component<components::input>(username_input, _game, *username_text);
                _game.emplace_component<components::button>(username_input, _game, abstractions::vector<float>{400, 40}, [&username](){
                    username.has_focus = true;
                });
                _game.emplace_component<ecs::components::gui::drawable>(username_input, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element({400, 40}, abstractions::gui::color::transparent)},
                        {static_cast<ecs::entity>(_game), std::move(username_text)}
                    })
                });

                // Input rectangle for Password
                auto password_input = _game.create_entity();
                _game.emplace_component<ecs::components::position>(password_input, (_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 40.0f);
                auto password_text = _game.display.factory->make_element("", _game.asset_manager.get("arial"), 24);
                auto &password = _game.emplace_component<components::input>(password_input, _game, *password_text);
                _game.emplace_component<components::button>(password_input, _game, abstractions::vector<float>{400, 40}, [&password](){
                    password.has_focus = true;
                });
                _game.emplace_component<ecs::components::gui::drawable>(password_input, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element({400, 40}, abstractions::gui::color::transparent)},
                        {static_cast<ecs::entity>(_game), std::move(password_text)}
                    })
                });

                // Input rectangle for Server Address
                auto server_address_input = _game.create_entity();
                _game.emplace_component<ecs::components::position>(server_address_input, (_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 + 60.0f);
                auto server_address_text = _game.display.factory->make_element("", _game.asset_manager.get("arial"), 24);
                auto &server_address = _game.emplace_component<components::input>(server_address_input, _game, *server_address_text);
                _game.emplace_component<components::button>(server_address_input, _game, abstractions::vector<float>{400, 40}, [&server_address](){
                    server_address.has_focus = true;
                });
                _game.emplace_component<ecs::components::gui::drawable>(server_address_input, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element({400, 40}, abstractions::gui::color::transparent)},
                        {static_cast<ecs::entity>(_game), std::move(server_address_text)}
                    })
                });

                // Input rectangle for Port
                auto port_input = _game.create_entity();
                _game.emplace_component<ecs::components::position>(port_input, (_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 + 160.0f);
                auto port_text = _game.display.factory->make_element("", _game.asset_manager.get("arial"), 24);
                auto &port = _game.emplace_component<components::input>(port_input, _game, *port_text);
                _game.emplace_component<components::button>(port_input, _game, abstractions::vector<float>{400, 40}, [&port](){
                    port.has_focus = true;
                });
                _game.emplace_component<ecs::components::gui::drawable>(port_input, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element({400, 40}, abstractions::gui::color::transparent)},
                        {static_cast<ecs::entity>(_game), std::move(port_text)}
                    })
                });

                _entities.push_back(username_input);
                _entities.push_back(password_input);
                _entities.push_back(server_address_input);
                _entities.push_back(port_input);

                // Title
                create_centered_text("Multiplayer", (_game.display.window->get_size().y - height) / 2, ecs::abstractions::gui::color::white, 48);

                // Username Text
                create_text({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 180.0f}, "Username :", ecs::abstractions::gui::color(255, 255, 255, 255 * 0.9), 24);

                // Input rectangle for Username
                create_rectangle({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 140.0f}, {400.0f, 40.0f}, ecs::abstractions::gui::color(30, 30, 30, 255 * 0.8), ecs::abstractions::gui::color::black);

                // Password Text
                create_text({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 80.0f}, "Password :", ecs::abstractions::gui::color(255, 255, 255, 255 * 0.9), 24);

                // Input rectangle for Password
                create_rectangle({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 40.0f}, {400.0f, 40.0f}, ecs::abstractions::gui::color(30, 30, 30, 255 * 0.8), ecs::abstractions::gui::color::black);

                // Server Address Text
                create_text({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 + 20.0f}, "Server Address :", ecs::abstractions::gui::color(255, 255, 255, 255 * 0.9), 24);

                // Input rectangle for Server Address
                create_rectangle({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 + 60.0f}, {400.0f, 40.0f}, ecs::abstractions::gui::color(30, 30, 30, 255 * 0.8), ecs::abstractions::gui::color::black);

                // Port Text
                create_text({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 + 120.0f}, "Port :", ecs::abstractions::gui::color(255, 255, 255, 255 * 0.9), 24);

                // Input rectangle for Port
                create_rectangle({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 + 160.0f}, {400.0f, 40.0f}, ecs::abstractions::gui::color(30, 30, 30, 255 * 0.8), ecs::abstractions::gui::color::black);

                // Connect Button
                create_centered_button("Connect", (_game.display.window->get_size().y - height) / 2 + height - buttonHeight - 20.0f, 24, ecs::abstractions::gui::color(14, 94, 255, 255),
                [this, &username, &password, &server_address, &port]() {
                    auto settings = _game.get_entity_component<components::settings>(_game);
                    auto network = _game.get_entity_component<components::network>(_game);

                    if (settings) { // TODO : Interact with the Client object
                        settings->get().username = username.content;
                        settings->get().password = password.content;
                        settings->get().server_address = server_address.content;
                        settings->get().port = port.content;

                        try {
                              network->get().client->connect(settings->get().server_address, std::stoi(settings->get().port));
//                              network.send_message("login " + settings->get().username + " " + settings->get().password);
                              network->get().client->send_message("login " + settings->get().username + " " + settings->get().password);

//                            _game.network_.connect(settings->get().server_address, std::stoi(settings->get().port));
//                            _game.network_.send_message("login " + settings->get().username + " " + settings->get().password);
                        } catch (const std::exception &e) {
                        }

                        _game.begin_scene(std::make_unique<game_scene>(_game));
                    }
                });

                // Background rectangle
                create_rectangle({(_game.display.window->get_size().x - width) / 2, (_game.display.window->get_size().y - height) / 2}, {width, height}, fillColor, outlineColor);
            }

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

            void create_rectangle(ecs::abstractions::vector<float> position, ecs::abstractions::vector<float> size, ecs::abstractions::gui::color color, ecs::abstractions::gui::color outlineColor = ecs::abstractions::gui::color::blue) noexcept
            {
                auto rectangle = _game.display.factory->make_element(size, color);
                rectangle->set_outline_color(outlineColor);
                rectangle->set_outline_thickness(2.0f);
                rectangle->position(position);

                auto drawable = _game.create_entity();
                _game.emplace_component<ecs::components::gui::drawable>(drawable, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), std::move(rectangle)}
                    })
                });
                _entities.push_back(drawable);

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


struct lobby_menu : public ecs::scene {
        constexpr lobby_menu(game &game) noexcept
            : ecs::scene(game), _game(game)
        {
        }
        protected:
            void create_entities() noexcept
            {
                initUI();
            }

        private:
            game &_game;

            void initUI() {
                float width = 500.0f;
                float height = 400.0f;

                // Title
                create_centered_text("Lobby", (_game.display.window->get_size().y - height) / 2, ecs::abstractions::gui::color::white, 48);

                auto join_lobby_input = _game.create_entity();
                _game.emplace_component<ecs::components::position>(join_lobby_input, (_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 20.0f);
                auto join_lobby_text = _game.display.factory->make_element("", _game.asset_manager.get("arial"), 24);
                auto &join_lobby = _game.emplace_component<components::input>(join_lobby_input, _game, *join_lobby_text);
                _game.emplace_component<components::button>(join_lobby_input, _game, abstractions::vector<float>{400, 40}, [&join_lobby](){
                    join_lobby.has_focus = true;
                });
                _game.emplace_component<ecs::components::gui::drawable>(join_lobby_input, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element({400, 40}, ecs::abstractions::gui::color::transparent)},
                        {static_cast<ecs::entity>(_game), std::move(join_lobby_text)}
                    })
                });

                create_rectangle({(_game.display.window->get_size().x - width) / 2, (_game.display.window->get_size().y - height) / 2}, {width, height}, ecs::abstractions::gui::color(100, 100, 100, 255 * 0.7), ecs::abstractions::gui::color::blue);

                // Start Button
                create_button("Start", {(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 + 100.0f}, {400, 40}, ecs::abstractions::gui::color(14, 94, 255, 255),
                    [&](){ // TODO : interact with the Client object
                        auto network = _game.get_entity_component<components::network>(_game);

                        network->get().client->send_message("start");
                    });

                _entities.push_back(join_lobby_input);

                // Join lobby code text
                create_text({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 60.0f}, "Join Lobby Code :", ecs::abstractions::gui::color(255, 255, 255, 255 * 0.9), 24);
                // Rectangle for Join lobby code
                create_rectangle({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 20.0f}, {400.0f, 40.0f}, ecs::abstractions::gui::color(30, 30, 30, 255 * 0.8), ecs::abstractions::gui::color::black);
                // Join Lobby Button
                create_button("Join Lobby", {(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 + 40.0f}, {400, 40}, ecs::abstractions::gui::color(14, 94, 255, 255),
                    [&](){
                        auto network = _game.get_entity_component<components::network>(_game);

                        std::cout << "Join Lobby Code: " << join_lobby.content << std::endl; // TODO : Interact with the Client object
//                        _game.network_.send_message("join_lobby " + join_lobby.content);
                        network->get().client->send_message("join_lobby " + join_lobby.content);

                    });
                // Create Lobby Button
                auto text = _game.display.factory->make_element("Create Lobby", _game.asset_manager.get("arial"), 24);
                ecs::abstractions::vector<float> text_size = {text->bounds(true).width, text->bounds(true).height};
                float button_height = 50.0f;
                ecs::abstractions::vector<float> button_size = {text_size.x + 40, button_height};

                create_button("Create Lobby", {(_game.display.window->get_size().x) / 2.0f - button_size.x - 20, (_game.display.window->get_size().y - height - 20.0f) / 2 + height - 50.0f}, 24, ecs::abstractions::gui::color(14, 94, 255, 255),
                    [&](){
                        auto network = _game.get_entity_component<components::network>(_game);

                        std::cout << "Create Lobby" << std::endl; // TODO : Interact with the Client object
//                        _game.network_.send_message("create_lobby");
                        network->get().client->send_message("create_lobby");
                    });

                // Join MatchMaking Button
                auto text2 = _game.display.factory->make_element(" Join Match ", _game.asset_manager.get("arial"), 24);
                ecs::abstractions::vector<float> text_size2 = {text2->bounds(true).width, text2->bounds(true).height};
                ecs::abstractions::vector<float> button_size2 = {text_size2.x + 40, button_height};

                create_button(" Join Match ", {(_game.display.window->get_size().x) / 2.0f + 20, (_game.display.window->get_size().y - height - 20.0f) / 2 + height - 50.0f}, 24, ecs::abstractions::gui::color(14, 94, 255, 255),
                    [&](){
                        auto settings = _game.get_entity_component<components::settings>(_game);
                        auto network = _game.get_entity_component<components::network>(_game);

                        std::cout << "Join Matchmaking with Username: " << settings->get().username << std::endl << "Password: " << settings->get().password << std::endl << "Server Address: " << settings->get().server_address << std::endl << "Port: " << settings->get().port << std::endl;
//                        _game.network_.send_message("matchmaking");
                        network->get().client->send_message("matchmaking");
                    }); // TODO : Interact with the Client object

                // background rectangle
                create_rectangle({(_game.display.window->get_size().x - width) / 2, (_game.display.window->get_size().y - height) / 2}, {width, height}, ecs::abstractions::gui::color(100, 100, 100, 255 * 0.7), ecs::abstractions::gui::color::blue);
            }


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

            int create_button(const std::string &label, ecs::abstractions::vector<float> position, int fontSize, ecs::abstractions::gui::color button_color, std::function<void()> action) noexcept
            {
                auto text = _game.display.factory->make_element(label, _game.asset_manager.get("arial"), fontSize);
                float text_top = text->bounds(true).y;
                float button_height = 50.0f;

                ecs::abstractions::vector<float> text_size = {text->bounds(true).width, text->bounds(true).height};
                ecs::abstractions::vector<float> button_size = {text_size.x + 40, button_height};
                create_text({position.x + (button_size.x - text_size.x) / 2, position.y + text_top}, label, ecs::abstractions::gui::color::white, fontSize);
                create_clickable_rectangle({position.x, position.y}, button_size, button_color, action);

                return button_size.y;
            }

            void create_rectangle(ecs::abstractions::vector<float> position, ecs::abstractions::vector<float> size, ecs::abstractions::gui::color color, ecs::abstractions::gui::color outlineColor = ecs::abstractions::gui::color::blue) noexcept
            {
                auto rectangle = _game.display.factory->make_element(size, color);
                rectangle->set_outline_color(outlineColor);
                rectangle->set_outline_thickness(2.0f);
                rectangle->position(position);

                auto drawable = _game.create_entity();
                _game.emplace_component<ecs::components::gui::drawable>(drawable, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), std::move(rectangle)}
                    })
                });
                _entities.push_back(drawable);

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

            void create_button(const std::string &label, ecs::abstractions::vector<float> position, ecs::abstractions::vector<float> size, ecs::abstractions::gui::color color, std::function<void()> action) noexcept
            {
                create_centered_text(label, position.y, ecs::abstractions::gui::color::white, 24);
                auto button = _game.create_entity();
                _game.emplace_component<ecs::components::position>(button, position.x, position.y);
                _game.emplace_component<ecs::components::gui::drawable>(button, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), _game.display.factory->make_element(size, color)}
                    })
                });
                _game.emplace_component<components::button>(button, _game, size, action);
                _entities.push_back(button);
            }
    };

    struct menu : public ecs::scene {
        constexpr menu(game &game) noexcept
            : ecs::scene(game), _game(game)
        {
            _game.emplace_component<components::settings>(_game, "username", "password", "server_address", "port", true);
        }

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
                    game.begin_scene(std::make_unique<multiplayer_menu>(game));
                });
                start_y += button_height + spacing;

                button_height = create_centered_button("Lobby", start_y, fontSize, button_color, [&game = _game](){
                    game.begin_scene(std::make_unique<lobby_menu>(game));
                });
                start_y += button_height + spacing;

                button_height = create_centered_button("Options", start_y, fontSize, button_color, [&game = _game](){
                    game.begin_scene(std::make_unique<game_scene>(game));
                });
                start_y += button_height + spacing;

                button_height = create_centered_button("Map Editor", start_y, fontSize, button_color, [&game = _game](){
                    game.begin_scene(std::make_unique<game_scene>(game));
                });
                start_y += button_height + spacing;

                button_height = create_centered_button("Game Options (ON/OFF Shader)", start_y, fontSize, button_color, [&game = _game](){
                    auto settings = game.get_entity_component<components::settings>(game);
                    settings->get().isShaderEnabled = !settings->get().isShaderEnabled;
                    std::cout << "Shader is now " << (settings->get().isShaderEnabled ? "enabled" : "disabled") << std::endl;
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
