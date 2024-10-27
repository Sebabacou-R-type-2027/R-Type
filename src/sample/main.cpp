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
        return e;
    }

    static constexpr components::gui::asset_manager &loadAssets(components::gui::asset_manager &in) noexcept {
        in.load_font("arial", "assets/fonts/arial.ttf");
        in.load_texture("ship", "assets/Ship/Ship.png");
        in.load_texture("enemy", "assets/r-type-enemy.gif");
        in.load_texture("enemy_chaser", "assets/Chasing_enemy/r-typesheet11_right.gif");
        in.load_texture("enemy_spawner", "assets/sprites/r-typesheet24.gif");
        in.load_texture("enemy_shooter", "assets/sprites/r-typesheet26.gif");
        return in;
    }

    entity initializePlayerComponents(entity e) noexcept {
        _registry.emplace_component<components::position>(e, 50.0f, 50.0f);
        _registry.emplace_component<components::engine::velocity>(e, 0.1f, 0.2f);
        auto label = std::make_shared<sf::Text>("Player", _assetManager.get_font("arial"));
        label->setOrigin(label->getGlobalBounds().left, label->getGlobalBounds().height);
        _registry.emplace_component<components::gui::drawable>(e, components::gui::drawable{_game,
            std::container<components::gui::drawable::elements_container>::make({
                {_game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Text>("Player", _assetManager.get_font("arial"), 12), "arial")},
                {_game, std::make_unique<ecs::components::gui::display_element>(
                    std::make_unique<sf::Sprite>(_assetManager.get_texture("ship")), "ship")}
            })
        });
        return e;
    }

    public:
        Game() noexcept
            : _registry(),
            _game(initializeGameComponents(_registry.create_entity())),
            _assetManager(loadAssets(_registry.add_component<components::gui::asset_manager>(_game, {}))),
            _player(initializePlayerComponents(_registry.create_entity())),
            _sigHandler(std::bind_front(&Game::stop, this))
        {
            std::cout << "Press Ctrl+C to stop" << std::endl;
            std::signal(SIGINT, _sigHandler.target<void(int)>());
            _registry.register_system<components::gui::window>(systems::gui::clear);
            _registry.register_system<const components::gui::drawable>(systems::gui::draw);
            _registry.register_system<components::gui::drawable, const components::position>(systems::gui::reposition);
            _registry.register_system<projectile_launcher, const components::position>(launch_projectile);
            _registry.register_system<const projectile>(cull_projectiles);
            systems::position_logger logger(_registry);
            _registry.register_system<components::position, const components::engine::velocity>(systems::engine::movement);
            _registry.register_system<components::gui::window>(systems::gui::display);
            auto launcher = _registry.create_entity();
            _registry.emplace_component<components::position>(launcher, 700.0f, 100.0f);
            _registry.add_component<projectile_launcher>(launcher, {1s, std::chrono::steady_clock::now(), _game});
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
