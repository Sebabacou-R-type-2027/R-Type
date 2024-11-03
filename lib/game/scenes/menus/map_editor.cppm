export module game:scenes.menus.map_editor;
import :game;
import :scenes.game;

import std;
import ecs;
import utils;

export namespace game::scenes {
    struct map_editor : public ecs::scene {
        constexpr map_editor(game &game) noexcept
            : ecs::scene(game), _game(game)
        {
            _game.emplace_component<components::map_editor>(_game, 1, "enemy", std::vector<std::string>{"enemy", "enemy_chaser"}, std::map<int, std::vector<std::pair<std::string, ecs::abstractions::vector<float>>>>
            {
                {1, {}},
                {2, {}},
                {3, {}},
                {4, {}},
                {5, {}},
                {6, {}},
                {7, {}},
                {8, {}},
                {9, {}},
                {10,{}}

            }, _game);
        }

        protected:
            void create_entities() noexcept
            {
                initUI();
            }

        private:
            void init_grid()
            {
                ecs::abstractions::vector<float> window_size = _game.display.window->get_size();
                ecs::abstractions::vector<float> grid_size = {window_size.x, window_size.y};
                ecs::abstractions::gui::color grid_color = ecs::abstractions::gui::color(255, 255, 255, 255 * 0.1);

                for (int i = 0; i < grid_size.x / grid_cell_size.x; i++)
                    create_rectangle_no_outline({i * grid_cell_size.x, 0.0f}, {1.0f, grid_size.y}, grid_color);
                for (int i = 0; i < grid_size.y / grid_cell_size.y; i++)
                    create_rectangle_no_outline({0.0f, i * grid_cell_size.y}, {grid_size.x, 1.0f}, grid_color);
            }
            void initUI() {
                init_grid();
            }

            void create_rectangle_no_outline(ecs::abstractions::vector<float> position, ecs::abstractions::vector<float> size, ecs::abstractions::gui::color color) noexcept
            {
                auto rectangle = _game.display.factory->make_element(size, color);
                rectangle->position(position);

                auto drawable = _game.create_entity();
                _game.emplace_component<ecs::components::gui::drawable>(drawable, ecs::components::gui::drawable{_game,
                    std::container<ecs::components::gui::drawable::elements_container>::make({
                        {static_cast<ecs::entity>(_game), std::move(rectangle)}
                    })
                });
                _entities.push_back(drawable);
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

            ecs::abstractions::vector<float> grid_cell_size = {32.0f, 32.0f};

            game &_game;
    };
}