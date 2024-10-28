#include <csignal>
#if __cpp_lib_modules < 202207L
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#endif

#include <SFML/Graphics.hpp>

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;
import game;

using namespace ecs;
using namespace game::components;
using namespace game::systems;

using namespace std::chrono_literals;

class Game {
    registry _registry;
    entity _game;
    components::gui::asset_manager &_assetManager;
    entity _player;

    std::function<void(int)> _sigHandler;

    void stop(int) noexcept
    {
        _registry.get_entity_component<ecs::components::gui::window>(_game)->get().window->close();
    }

    constexpr entity initializeGameComponents(entity e) noexcept {
        _registry.emplace_component<components::gui::window>(e, std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), "Game"));
        _registry.emplace_component<components::gui::animation_clock>(e);
        return e;
    }

    static constexpr components::gui::asset_manager &loadAssets(components::gui::asset_manager &in) noexcept {
        in.load_font("arial", "assets/fonts/arial.ttf");
        in.load_texture("ship", "assets/Player/Player.gif");
        in.load_texture("enemy", "assets/r-type-enemy.gif");
        in.load_texture("enemy_chaser", "assets/Chasing_enemy/r-typesheet11_right.gif");
        in.load_texture("enemy_spawner", "assets/sprites/r-typesheet24.gif");
        in.load_texture("enemy_shooter", "assets/r-typesheet26.gif");
        in.load_texture("bullet", "assets/Bullets/01.png");
        return in;
    }

    static game::components::button create_button(sf::Vector2f position, sf::Vector2f size,
        const sf::Color& defaultColor, const sf::Color& hoverColor, const sf::Color& clickColor,
        std::string_view text, const sf::Font& font, const sf::Color& textColor, std::size_t text_size,
        std::function<void()> action, ecs::entity window) noexcept
    {
        sf::RectangleShape shape(size);
        shape.setFillColor(defaultColor);
        shape.setPosition(position);

        sf::Text buttonText(std::string(text), font, text_size);
        buttonText.setFillColor(textColor);
        buttonText.setOrigin(buttonText.getGlobalBounds().width / 2, buttonText.getGlobalBounds().height / 2 + buttonText.getGlobalBounds().top);
        buttonText.setPosition(position);

        game::components::button btn = button(shape, buttonText, action, window);
        btn.defaultColor = defaultColor;
        btn.hoverColor = hoverColor;
        btn.clickColor = clickColor;

        return btn;
    }

    entity initializePlayerComponents(entity e) noexcept {
        _registry.emplace_component<components::position>(e, 50.0f, 50.0f);
        _registry.emplace_component<components::engine::velocity>(e, 0.0f, 0.0f);
        _registry.emplace_component<components::engine::controllable>(e, true, 10.0f);
        _registry.add_component<projectile_launcher>(e, {1s, std::chrono::steady_clock::now(), _game});
        auto label = std::make_shared<sf::Text>("Play", _assetManager.get_font("arial"));
        label->setOrigin(label->getGlobalBounds().left, label->getGlobalBounds().height);
        _registry.emplace_component<components::gui::drawable>(e, components::gui::drawable{_game,
            std::container<components::gui::drawable::elements_container>::make({
                {_game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Player", _assetManager.get_font("arial"), 12), "arial")},
                {_game, std::make_unique<ecs::components::gui::animation>(
                    _assetManager.get_texture("ship"), 1, 5, 10ms, "ship")}
            })
        });
        return e;
    }

    void spawn_enemy(const ecs::components::position& position)
    {
        const ecs::components::gui::asset_manager &asset_manager = *_registry.get_entity_component<const ecs::components::gui::asset_manager>(_game);
        auto e = _registry.create_entity();
        _registry.add_component(e, enemy{1, 1, std::chrono::steady_clock::now()});
        _registry.add_component(e, ecs::components::position{position.x, position.y});
        _registry.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
        _registry.add_component(e, enemy_loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 1.0f, 0.0f, 100.0f, 2.0f});
        _registry.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {_game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Enemy", asset_manager.get_font("arial"), 12), "arial")},
                {_game, std::make_unique<ecs::components::gui::animation>
                    (asset_manager.get_texture("enemy"), 1, 8, 10ms, "enemy")
                }
            })
        });
    }

    void spawn_enemy_chaser(ecs::entity target, const ecs::components::position& position, ecs::entity game)
    {
        const ecs::components::gui::asset_manager &asset_manager = *_registry.get_entity_component<const ecs::components::gui::asset_manager>(game);
        auto e = _registry.create_entity();
        _registry.add_component(e, enemy{100, 10, std::chrono::steady_clock::now()});
        _registry.add_component(e, ecs::components::position{position.x, position.y});
        _registry.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
        _registry.add_component(e, enemy_chaser{target, 10.0f});
        _registry.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {_game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Chaser", asset_manager.get_font("arial"), 12), "arial")},
                {_game, std::make_unique<ecs::components::gui::animation>
                    (asset_manager.get_texture("enemy_chaser"), 1, 3, 10ms, "enemy_chaser")
                }
            })
        });
    }

    void spawn_enemy_spawner(const ecs::components::position& position, ecs::entity game)
    {
        const ecs::components::gui::asset_manager &asset_manager = *_registry.get_entity_component<const ecs::components::gui::asset_manager>(game);
        auto e = _registry.create_entity();
        _registry.add_component(e, enemy{100, 10, std::chrono::steady_clock::now()});
        _registry.add_component(e, ecs::components::position{position.x, position.y});
        _registry.add_component(e, ecs::components::engine::velocity{0.0f, 0.0f});
        _registry.add_component(e, enemy_spawner{2.0f, 5, std::chrono::steady_clock::now(), game});
        _registry.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {_game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Spawner", asset_manager.get_font("arial"), 12), "arial")},
                {_game, std::make_unique<ecs::components::gui::animation>
                    (asset_manager.get_texture("enemy_spawner"), 1, 5, 10ms, "enemy_spawner")
                }
            })
        });
    }


    void spawn_enemy_shooter(const ecs::components::position& position, ecs::entity game)
    {
        const ecs::components::gui::asset_manager &asset_manager = *_registry.get_entity_component<const ecs::components::gui::asset_manager>(game);
        auto e = _registry.create_entity();
        _registry.add_component(e, enemy{100, 10, std::chrono::steady_clock::now()});
        _registry.add_component(e, ecs::components::position{position.x, position.y});
        _registry.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
        _registry.add_component(e, enemy_shooter{2.0f, std::chrono::steady_clock::now(), game});
        _registry.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{_game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {_game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Shooter", asset_manager.get_font("arial"), 12), "arial")},
                {_game, std::make_unique<ecs::components::gui::animation>
                    (asset_manager.get_texture("enemy_shooter"), 1, 3, 10ms, "enemy_shooter")
                }
            })
        });
    }


    void initializeEnemies() noexcept {
        // spawn_enemy({100.0f, 100.0f});
        spawn_enemy_chaser(_player, {200.0f, 200.0f}, _game);
    //    spawn_enemy_spawner({300.0f, 300.0f}, _game);
    //    spawn_enemy_shooter(1s, {400.0f, 400.0f}, _game);
    }

    public:
        Game() noexcept
            : _registry(),
            _game(initializeGameComponents(_registry.create_entity())),
            _assetManager(loadAssets(_registry.add_component<components::gui::asset_manager>(_game, {}))),
            _player(initializePlayerComponents(_registry.create_entity())),
            _sigHandler(std::bind_front(&Game::stop, this))
            {
            initializeEnemies();
            std::cout << "Press Ctrl+C to stop" << std::endl;
            std::signal(SIGINT, _sigHandler.target<void(int)>());
            _registry.register_system<components::gui::window>(systems::gui::clear);
            _registry.register_system<components::gui::animation_clock>(systems::gui::update_clock);
            _registry.register_system<const components::gui::drawable>(systems::gui::draw);
            _registry.register_system<game::components::button>(render_button);
            _registry.register_system<components::gui::drawable, const components::position>(systems::gui::reposition);
            _registry.register_system<projectile_launcher, const components::position>(launch_projectile);
            _registry.register_system<enemy_shooter, const components::position>(move_enemy_shooter);
            _registry.register_system<enemy_loop_movement, components::position>(move_enemy_loop);
            _registry.register_system<enemy_chaser, components::position>(move_enemy_chaser);
            _registry.register_system<enemy_spawner, components::position>(handle_enemy_spawner);
            _registry.register_system<const projectile>(cull_projectiles);
            _registry.register_system<game::components::button>(handle_button);
            systems::position_logger logger(_registry);
            _registry.register_system<components::position, const components::engine::velocity>(systems::engine::movement);
            _registry.register_system<components::position, const components::engine::controllable>(systems::engine::control);
            _registry.register_system<components::gui::window>(systems::gui::display);
            _registry.register_system<components::gui::animation_clock>(systems::gui::reset_clock);
            auto button = _registry.create_entity();
            projectile_launcher &proj_launcher = *_registry.get_entity_component<projectile_launcher>(_player);
            _registry.add_component(button, create_button({700, 200}, {100, 50},
                    sf::Color::Blue, sf::Color::Cyan, sf::Color::Green,
                    "Fire", _assetManager.get_font("arial"), sf::Color::White, 50,
                    [&proj_launcher](){ proj_launcher.last_shot = std::chrono::steady_clock::time_point(0s); }, _game));
        }

        void run() noexcept {
            sf::RenderWindow &window = *_registry.get_entity_component<ecs::components::gui::window>(_game)->get().window;
            sf::Event event;
            signal(SIGINT, _sigHandler.target<void(int)>());
            auto next = std::chrono::steady_clock::now();
            while (window.isOpen()) {
                while (window.pollEvent(event))
                    if (event.type == sf::Event::Closed)
                        window.close();
                if (next >= std::chrono::steady_clock::now())
                    continue;
                next += 50ms;
                _registry.run_systems();
            }
        }
};

int main()
{
    Game game;
    game.run();
    return 0;
}
