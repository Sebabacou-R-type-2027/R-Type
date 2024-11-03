export module game:scenes.menus.lobby;
import :game;

import std;
import ecs;
import utils;

export namespace game::scenes {
    struct lobby_menu : public ecs::scene {
        constexpr lobby_menu(game &game) noexcept
            : ecs::scene(game), _game(game)
        {
        }
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

            void initUI() {
                float width = 500.0f;
                float height = 400.0f;

                // Title
                /**
                    * @brief Create the UI

                    * This function is used to create the UI of the scene.
                 */
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

                _entities.push_back(join_lobby_input);

                // Join lobby code text
                create_text({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 60.0f}, "Join Lobby Code :", ecs::abstractions::gui::color(255, 255, 255, 255 * 0.9), 24);
                // Rectangle for Join lobby code
                create_rectangle({(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 - 20.0f}, {400.0f, 40.0f}, ecs::abstractions::gui::color(30, 30, 30, 255 * 0.8), ecs::abstractions::gui::color::black);
                // Join Lobby Button
                create_button("Join Lobby", {(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 + 40.0f}, {400, 40}, ecs::abstractions::gui::color(14, 94, 255, 255),
                    [&](){
                        auto network = _game.get_entity_component<components::network>(_game);
                        network->get().client->send_message("join_lobby " + join_lobby.content);
                    });

                create_button("Start Game", {(_game.display.window->get_size().x - 400.0f) / 2, (_game.display.window->get_size().y - 40.0f) / 2 + 100.0f}, {400, 40}, ecs::abstractions::gui::color(14, 94, 255, 255),
                    [&](){
                        auto network = _game.get_entity_component<components::network>(_game);
                        network->get().client->send_message("start");
                    });

                // Create Lobby Button
                auto text = _game.display.factory->make_element("Create Lobby", _game.asset_manager.get("arial"), 24);
                ecs::abstractions::vector<float> text_size = {text->bounds(true).width, text->bounds(true).height};
                float button_height = 50.0f;
                ecs::abstractions::vector<float> button_size = {text_size.x + 40, button_height};

                create_button("Create Lobby", {(_game.display.window->get_size().x) / 2.0f - button_size.x - 20, (_game.display.window->get_size().y - height - 20.0f) / 2 + height - 50.0f}, 24, ecs::abstractions::gui::color(14, 94, 255, 255),
                    [&](){
                        auto network = _game.get_entity_component<components::network>(_game);
                        network->get().client->send_message("create_lobby");
//                        std::cout << "Create Lobby" << std::endl;
                    });

                // Join MatchMaking Button
                auto text2 = _game.display.factory->make_element(" Join Match ", _game.asset_manager.get("arial"), 24);
                ecs::abstractions::vector<float> text_size2 = {text2->bounds(true).width, text2->bounds(true).height};
                ecs::abstractions::vector<float> button_size2 = {text_size2.x + 40, button_height};

                create_button(" Join Match ", {(_game.display.window->get_size().x) / 2.0f + 20, (_game.display.window->get_size().y - height - 20.0f) / 2 + height - 50.0f}, 24, ecs::abstractions::gui::color(14, 94, 255, 255),
                    [&](){
                        auto settings = _game.get_entity_component<components::settings>(_game);
                        auto network = _game.get_entity_component<components::network>(_game);
                        network->get().client->send_message("matchmaking");
//                        std::cout << "Join Matchmaking with Username: " << settings->get().username << std::endl << "Password: " << settings->get().password << std::endl << "Server Address: " << settings->get().server_address << std::endl << "Port: " << settings->get().port << std::endl;
                    });

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
}
