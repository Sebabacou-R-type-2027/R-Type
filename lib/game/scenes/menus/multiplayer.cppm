export module game:scenes.menus.multiplayer;
import :game;
import :scenes.game;

import std;
import ecs;
import utils;

export namespace game::scenes {
    struct multiplayer_menu : public ecs::scene {
        constexpr multiplayer_menu(game &game) noexcept
            : ecs::scene(game), _game(game)
        {}

        protected:
            /**
                * @brief Create the entities

                * This function is used to create the entities of the scene.

                * @return The entities of the scene
             */
            void create_entities() noexcept
            {
                initUI();
            }

        private:
            game &_game;

            /**
                * @brief Initialize the UI

                * This function is used to initialize the UI of the scene.
             */
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

                    if (settings) {
                        settings->get().username = username.content;
                        settings->get().password = password.content;
                        settings->get().server_address = server_address.content;
                        settings->get().port = port.content;
                        auto network = _game.get_entity_component<components::network>(_game);
                        try {
                            network->get().network->connect(settings->get().server_address, settings->get().port);
                        } catch (const std::exception &e) {
                            std::cerr << e.what() << std::endl;
                            return;
                        }

                        _game.begin_scene(std::make_unique<game_scene>(_game));
                    }
                });

                // Background rectangle
                create_rectangle({(_game.display.window->get_size().x - width) / 2, (_game.display.window->get_size().y - height) / 2}, {width, height}, fillColor, outlineColor);
            }

            /**
                * @brief Create a centered button

                * This function is used to create a centered button.

                * @param label The label of the button
                * @param yPos The y position of the button
                * @param fontSize The font size of the button
                * @param button_color The color of the button
                * @param action The action of the button
                * @return The height of the button
             */
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

            /**
                * @brief Create a rectangle

                * This function is used to create a rectangle.

                * @param position The position of the rectangle
                * @param size The size of the rectangle
                * @param color The color of the rectangle
                * @param outlineColor The outline color of the rectangle
             */
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

            /**
                * @brief Create text

                * This function is used to create text.

                * @param position The position of the text
                * @param text The text
                * @param color The color of the text
                * @param size The size of the text
             */
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

            /**
                * @brief Create a centered text

                * This function is used to create a centered text.

                * @param label The label of the text
                * @param yPos The y position of the text
                * @param color The color of the text
                * @param fontSize The font size of the text
             */
            void create_centered_text(std::string label, float yPos, ecs::abstractions::gui::color color, int fontSize = 24) noexcept
            {
                ecs::abstractions::vector<float> window_size = _game.display.window->get_size();
                auto text = _game.display.factory->make_element(label, _game.asset_manager.get("arial"), fontSize);
                float text_top = text->bounds(true).y;

                ecs::abstractions::vector<float> text_size = {text->bounds(true).width, text->bounds(true).height};
                create_text({(window_size.x - text_size.x) / 2, yPos + text_top}, label, color, fontSize);
            }

            /**
                * @brief Create a button

                * This function is used to create a button.

                * @param label The label of the button
                * @param position The position of the button
                * @param size The size of the button
                * @param color The color of the button
                * @param action The action of the button
             */
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
