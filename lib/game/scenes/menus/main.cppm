export module game:scenes.menus.main;
import :game;
import :scenes.game;
import :scenes.menus.lobby;
import :scenes.menus.multiplayer;
import :scenes.menus.map_editor;

import std;
import ecs;
import utils;

export namespace game::scenes {
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
                    game.begin_scene(std::make_unique<map_editor>(game));
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
