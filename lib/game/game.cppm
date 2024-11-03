#if __cpp_lib_modules < 202207L
module;

#include <chrono>
#include <memory>
#endif
export module game:game;
export import :components.buttons;
export import :components.projectiles;
export import :components.enemies;
export import :components.inputs;
export import :components.settings;
export import :systems.buttons;
export import :systems.projectiles;
export import :systems.enemies;
export import :systems.inputs;
export import :systems.shader_background;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

export namespace game {
    using namespace components;
    using namespace systems;

    class game : public ecs::registry {
        ecs::entity _game;

        constexpr ecs::entity init_game(ecs::entity e) noexcept
        {
            this->emplace_component<ecs::components::gui::display>(e,
                std::make_unique<ecs::implementations::gui::SFML::window>(
                    ecs::abstractions::vector<std::uint32_t>(1920, 1080),
                    "R-Type"),
                std::make_unique<ecs::implementations::gui::SFML::element_factory>());
            this->emplace_component<ecs::components::gui::animation_clock>(e);
            auto &asset_manager = this->emplace_component<ecs::components::gui::asset_manager>(e,
                std::make_unique<ecs::implementations::gui::SFML::asset_loader>());
            load_assets(asset_manager);
            return e;
        }

        static constexpr void load_assets(ecs::components::gui::asset_manager &asset_manager) noexcept
        {
            asset_manager.load("arial", "assets/fonts/arial.ttf", "font");
            asset_manager.load("ship", "assets/Player/Player.gif", "texture");
            asset_manager.load("enemy", "assets/r-type-enemy.gif", "texture");
            asset_manager.load("enemy_chaser", "assets/Chasing_enemy/r-typesheet11_right.gif", "texture");
            asset_manager.load("enemy_spawner", "assets/sprites/r-typesheet24.gif", "texture");
            asset_manager.load("enemy_shooter", "assets/r-typesheet26.gif", "texture");
            asset_manager.load("bullet", "assets/Bullets/01.png", "texture");
        }

        constexpr void register_systems() noexcept
        {
            this->register_system<components::input>(handle_text_input);
            this->register_system<components::input, components::button>(reset_focus);
            this->register_system<const projectile>(cull_projectiles);
            this->register_system<projectile_launcher, const ecs::components::position>(launch_projectile);
            this->register_system<enemy_shooter, const ecs::components::position>(move_enemy_shooter);
            this->register_system<enemy_loop_movement, ecs::components::position>(move_enemy_loop);
            this->register_system<enemy_chaser, ecs::components::position>(move_enemy_chaser);
            this->register_system<enemy_spawner, ecs::components::position>(handle_enemy_spawner);
            this->register_system<button, const ecs::components::position>(press_button);
            this->register_gui_systems();
            this->register_system<components::settings, const ecs::components::gui::display>(shader_background);
            // this->register_system<ecs::components::gui::display>(ecs::systems::gui::draw_shader_background);
            this->register_system<ecs::components::gui::display>(ecs::systems::gui::clear);
            this->register_engine_systems();
        }

        public:
            ecs::components::gui::display &display;
            ecs::components::gui::asset_manager &asset_manager;
            std::chrono::steady_clock::duration tick_rate = 50ms;

            game() noexcept
                : _game(init_game(this->create_entity())),
                asset_manager(*this->get_entity_component<ecs::components::gui::asset_manager>(_game)),
                display(*this->get_entity_component<ecs::components::gui::display>(_game))
            {
                register_systems();
            }

            void run() noexcept
            {
                auto next = std::chrono::steady_clock::now();
                while (display.window->is_open()) {
                    next += tick_rate;
                    this->run_systems();
                    std::this_thread::sleep_until(next);
                }
            }

            constexpr operator ecs::entity() const noexcept { return _game; }
    };
}
