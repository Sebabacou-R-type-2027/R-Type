#if __cpp_lib_modules < 202207L
module;

#include <chrono>
#include <memory>
#endif
export module game;
export import :components.buttons;
export import :components.projectiles;
export import :components.enemies;
export import :systems.buttons;
export import :systems.projectiles;
export import :systems.enemies;

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
        ecs::components::gui::asset_manager &_asset_manager;

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

        inline ecs::entity init_player(ecs::entity e) noexcept
        {
            this->emplace_component<ecs::components::position>(e, 50.0f, 50.0f);
            this->emplace_component<ecs::components::engine::velocity>(e, 0.0f, 0.0f);
            this->emplace_component<ecs::components::engine::controllable>(e, _game, true, 10.0f);
            this->add_component<projectile_launcher>(e, {1s, std::chrono::steady_clock::now(), _game});
            auto label = display.factory->make_element("Player", _asset_manager.get("arial"), 12);
            label->set_origin({label->bounds(true).width / 2, label->bounds(true).height / 2});
            this->emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
                std::container<ecs::components::gui::drawable::elements_container>::make({
                    {_game, std::move(label)},
                    {_game, display.factory->make_element(
                        dynamic_cast<const ecs::abstractions::gui::texture &>(_asset_manager.get("ship")),
                        {5, 1})}
                })
            });
            return e;
        }

        constexpr void register_systems() noexcept
        {
            this->register_system<ecs::components::gui::display>(ecs::systems::gui::display);
            this->register_system<ecs::components::gui::animation_clock>(ecs::components::clock::reset_system);
            this->register_system<const ecs::components::gui::drawable>(ecs::systems::gui::draw);
            this->register_system<ecs::components::gui::animation_clock>(ecs::components::clock::cache_system);
            this->register_system<ecs::components::gui::drawable, const ecs::components::position>(ecs::systems::gui::reposition);
            this->register_system<ecs::components::gui::display>(ecs::systems::gui::clear);
            this->register_system<const projectile>(cull_projectiles);
            this->register_system<projectile_launcher, const ecs::components::position>(launch_projectile);
            this->register_system<enemy_shooter, const ecs::components::position>(move_enemy_shooter);
            this->register_system<enemy_loop_movement, ecs::components::position>(move_enemy_loop);
            this->register_system<enemy_chaser, ecs::components::position>(move_enemy_chaser);
            this->register_system<enemy_spawner, ecs::components::position>(handle_enemy_spawner);
            this->register_system<button, const ecs::components::position>(press_button);
            this->register_system<ecs::components::position, const ecs::components::engine::velocity>(ecs::systems::engine::movement);
            this->register_system<ecs::components::position, const ecs::components::engine::controllable>(ecs::systems::engine::control);
        }

        public:
            ecs::components::gui::display &display;
            const ecs::entity player;
            std::chrono::steady_clock::duration tick_rate = 50ms;

            game() noexcept
                : _game(init_game(this->create_entity())),
                _asset_manager(*this->get_entity_component<ecs::components::gui::asset_manager>(_game)),
                display(*this->get_entity_component<ecs::components::gui::display>(_game)),
                player(init_player(this->create_entity()))
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
