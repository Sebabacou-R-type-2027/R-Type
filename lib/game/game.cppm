module;

#include <csignal>
#if __cpp_lib_modules < 202207L
#include <chrono>
#include <functional>
#endif
#include <SFML/Graphics.hpp>
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
        ecs::components::gui::asset_manager &_assetManager;

        constexpr ecs::entity init_game(ecs::entity e) noexcept
        {
            this->emplace_component<ecs::components::gui::window>(e, std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), "Game"));
            this->emplace_component<ecs::components::gui::animation_clock>(e);
            return e;
        }

        static constexpr ecs::components::gui::asset_manager &load_assets(ecs::components::gui::asset_manager &asset_manager) noexcept
        {
            asset_manager.load_font("arial", "assets/fonts/arial.ttf");
            asset_manager.load_texture("ship", "assets/Player/Player.gif");
            asset_manager.load_texture("enemy", "assets/r-type-enemy.gif");
            asset_manager.load_texture("enemy_chaser", "assets/Chasing_enemy/r-typesheet11_right.gif");
            asset_manager.load_texture("enemy_spawner", "assets/sprites/r-typesheet24.gif");
            asset_manager.load_texture("enemy_shooter", "assets/r-typesheet26.gif");
            asset_manager.load_texture("bullet", "assets/Bullets/01.png");
            return asset_manager;
        }

        inline ecs::entity init_player(ecs::entity e) noexcept
        {
            this->emplace_component<ecs::components::position>(e, 50.0f, 50.0f);
            this->emplace_component<ecs::components::engine::velocity>(e, 0.0f, 0.0f);
            this->emplace_component<ecs::components::engine::controllable>(e, true, 10.0f);
            this->add_component<projectile_launcher>(e, {1.0f, 1s, std::chrono::steady_clock::now(), _game});
            auto label = std::make_shared<sf::Text>("Play", _assetManager.get_font("arial"));
            label->setOrigin(label->getGlobalBounds().left, label->getGlobalBounds().height);
            this->emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
                std::container<ecs::components::gui::drawable::elements_container>::make({
                    {_game, std::make_unique<ecs::components::gui::display_element>(
                        std::make_unique<sf::Text>("Player", _assetManager.get_font("arial"), 12), "arial")},
                    {_game, std::make_unique<ecs::components::gui::animation>(
                        _assetManager.get_texture("ship"), 1, 5, 10ms, "ship")}
                })
            });
            return e;
        }

        constexpr void register_systems() noexcept
        {
            this->register_system<ecs::components::gui::window>(ecs::systems::gui::clear);
            this->register_system<ecs::components::gui::animation_clock>(ecs::systems::gui::reset_clock);
            this->register_system<const button>(ecs::systems::gui::draw);
            this->register_system<const ecs::components::gui::drawable>(ecs::systems::gui::draw);
            this->register_system<ecs::components::gui::animation_clock>(ecs::systems::gui::update_clock);
            this->register_system<button, const ecs::components::position>(ecs::systems::gui::reposition);
            this->register_system<ecs::components::gui::drawable, const ecs::components::position>(ecs::systems::gui::reposition);
            this->register_system<projectile_launcher, const ecs::components::position>(launch_projectile);
            this->register_system<enemy_shooter, ecs::components::position, ecs::components::engine::velocity>(move_enemy_shooter);
            this->register_system<enemy_loop_movement, ecs::components::position>(move_enemy_loop);
            this->register_system<enemy_chaser, ecs::components::position>(move_enemy_chaser);
            this->register_system<enemy_spawner, ecs::components::position>(handle_enemy_spawner);
            this->register_system<const projectile>(cull_projectiles);
            this->register_system<button>(press_button);
            this->register_system<ecs::components::position, const ecs::components::engine::velocity>(ecs::systems::engine::movement);
            this->register_system<ecs::components::position, const ecs::components::engine::controllable>(ecs::systems::engine::control);
            this->register_system<ecs::components::gui::window>(ecs::systems::gui::display);
        }

        public:
            const ecs::entity player;
            std::chrono::steady_clock::duration tick_rate = 50ms;

            game() noexcept
                : _game(init_game(this->create_entity())),
                _assetManager(load_assets(this->add_component<ecs::components::gui::asset_manager>(_game, {}))),
                player(init_player(this->create_entity()))
            {
                register_systems();
            }

            void run() noexcept
            {
                sf::RenderWindow &window = *this->get_entity_component<ecs::components::gui::window>(_game)->get().window;
                sf::Event event;
                auto next = std::chrono::steady_clock::now();
                while (window.isOpen()) {
                    while (window.pollEvent(event))
                        if (event.type == sf::Event::Closed)
                            window.close();
                    if (next >= std::chrono::steady_clock::now())
                        continue;
                    next += tick_rate;
                    this->run_systems();
                }
            }

            constexpr operator ecs::entity() const noexcept { return _game; }
    };
}
