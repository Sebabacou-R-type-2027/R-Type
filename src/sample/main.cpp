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

using namespace ecs;
using namespace ecs::components;

class Game {
    registry _registry;
    entity _game;
    gui::asset_manager &_assetManager;
    entity _player;

    sf::RenderWindow _window;

    std::function<void(int)> _sigHandler;

    void stop(int) noexcept
    {
        _window.close();
    }

    constexpr entity initializeGameComponents(entity e) noexcept {
        _registry.add_component<ecs::components::gui::window>(e, {_window});
        return e;
    }

    static constexpr gui::asset_manager &loadAssets(gui::asset_manager &in) noexcept {
        in.load_font("arial", "assets/fonts/arial.ttf");
        in.load_texture("ship", "assets/Ship/Ship.png");
        return in;
    }

    entity initializePlayerComponents(entity e) noexcept {
        _registry.emplace_component<position>(e, 50.0f, 50.0f);
        _registry.emplace_component<engine::velocity>(e, 10.0f, 10.0f);
        _registry.emplace_component<engine::controllable>(e, true, 10.0f);
        auto label = std::make_shared<sf::Text>("Player", _assetManager.get_font("arial"));
        label->setOrigin(label->getGlobalBounds().left, label->getGlobalBounds().height);
        _registry.emplace_component<gui::drawable>(e, gui::drawable{_game,
            std::initializer_list<gui::drawable::elements_container::value_type>{
                {_game, {std::make_shared<sf::Text>("Player", _assetManager.get_font("arial"), 12), "arial"}},
                {_game, {std::make_shared<sf::Sprite>(_assetManager.get_texture("ship")), "ship"}}
            }
        });
        return e;
    }

    public:
        Game() noexcept
            : _registry(),
            _game(initializeGameComponents(_registry.create_entity())),
            _assetManager(loadAssets(_registry.add_component<ecs::components::gui::asset_manager>(_game, {}))),
            _player(initializePlayerComponents(_registry.create_entity())),
            _window(sf::VideoMode(1920, 1080), "Game"),
            _sigHandler(std::bind_front(&Game::stop, this))
        {
            std::cout << "Press Ctrl+C to stop" << std::endl;
            std::signal(SIGINT, _sigHandler.target<void(int)>());
            _registry.register_system<ecs::components::gui::window>(ecs::systems::gui::clear);
            _registry.register_system<const ecs::components::gui::drawable>(ecs::systems::gui::draw);
            _registry.register_system<ecs::components::engine::velocity, const ecs::components::engine::controllable>(ecs::systems::engine::control);
            _registry.register_system<ecs::components::gui::drawable, const ecs::components::position>(ecs::systems::gui::reposition);
            ecs::systems::position_logger logger(_registry);
            _registry.register_system<ecs::components::position, const ecs::components::engine::velocity>(ecs::systems::engine::movement);
            _registry.register_system<ecs::components::gui::window>(ecs::systems::gui::display);
        }

        void run() noexcept {
            sf::Event event;
            auto next = std::chrono::steady_clock::now();
            while (_window.isOpen()) {
                while (_window.pollEvent(event))
                    if (event.type == sf::Event::Closed)
                        _window.close();
                using namespace std::chrono_literals;
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
