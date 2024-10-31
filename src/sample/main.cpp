#if __cpp_lib_modules < 202207L
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#endif

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;
import game;

using namespace ecs;
using namespace game::components;

using namespace std::chrono_literals;

void spawn_enemy(const ecs::components::position& position, game::game &game)
{
    const ecs::components::gui::asset_manager &asset_manager =
        *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.add_component(e, enemy{1, 1, std::chrono::steady_clock::now()});
    game.add_component(e, ecs::components::position{position.x, position.y});
    game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
    game.add_component(e, enemy_loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 1.0f, 0.0f, 100.0f, 2.0f});
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), game.display.factory->make_element(
                "Enemy", asset_manager.get("arial"), 12)},
            {static_cast<ecs::entity>(game), game.display.factory->make_element(
                dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("enemy")), {8, 1}, 10ms)}
        })
    });
}

static void spawn_enemy_chaser(ecs::entity target, const ecs::components::position& position, game::game &game)
{
    const ecs::components::gui::asset_manager &asset_manager =
        *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.add_component(e, enemy{100, 10, std::chrono::steady_clock::now()});
    game.add_component(e, ecs::components::position{position.x, position.y});
    game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
    game.add_component(e, enemy_chaser{target, 10.0f});
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), game.display.factory->make_element(
                "Chaser", asset_manager.get("arial"), 12)},
            {static_cast<ecs::entity>(game), game.display.factory->make_element(
                dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("enemy_chaser")), {3, 1}, 10ms)}
        })
    });
}

static void spawn_enemy_spawner(const ecs::components::position& position, game::game &game)
{
    const auto now = std::chrono::steady_clock::now();
    const ecs::components::gui::asset_manager &asset_manager =
        *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.emplace_component<enemy>(e, 100, 10, now);
    game.emplace_component<enemy_spawner>(e, 2s, 5ul, game);
    game.add_component(e, position);
    game.emplace_component<ecs::components::engine::velocity>(e);
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), game.display.factory->make_element(
                "Spawner", asset_manager.get("arial"), 12)},
            {static_cast<ecs::entity>(game), game.display.factory->make_element(
                dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("enemy_spawner")), {5, 1}, 10ms)}
        })
    });
}

static void spawn_enemy_shooter(const ecs::components::position& position, game::game &game)
{
    const ecs::components::gui::asset_manager &asset_manager =
        *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.add_component(e, enemy{100, 10, std::chrono::steady_clock::now()});
    game.add_component(e, ecs::components::position{position.x, position.y});
    game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
    game.add_component(e, enemy_shooter{2s, game});
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), game.display.factory->make_element(
                "Shooter", asset_manager.get("arial"), 12)},
            {static_cast<ecs::entity>(game), game.display.factory->make_element(
                dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("enemy_shooter")), {3, 1}, 10ms)}
        })
    });
}


static void initializeEnemies(game::game &game) noexcept {
    spawn_enemy({100.0f, 100.0f}, game);
    spawn_enemy_chaser(game.player, {200.0f, 200.0f}, game);
    spawn_enemy_spawner({300.0f, 300.0f}, game);
    spawn_enemy_shooter({400.0f, 400.0f}, game);
}

int main()
{
    game::game game;
    initializeEnemies(game);

    projectile_launcher &launcher = *game.get_entity_component<projectile_launcher>(game.player);
    const ecs::components::gui::asset_manager &asset_manager =
        *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto button = game.create_entity();
    game.emplace_component<components::position>(button, 230.f, 875.f);
    game.emplace_component<game::components::button>(button, game, ecs::abstractions::vector<float>(100, 50),
            [&launcher](){ launcher.last_shot = std::chrono::steady_clock::time_point(0s); });
    auto label = game.display.factory->make_element(
        "Fire", asset_manager.get("arial"), 48);
    label->set_origin({label->bounds(true).width / 2, label->bounds(true).height / 2});
    label->set_repositioning(ecs::abstractions::gui::drawable_element::reposition_center);
    auto background = game.display.factory->make_element({100, 50},
        ecs::abstractions::gui::color::cyan);
    background->set_origin({background->bounds(true).width / 2, background->bounds(true).height / 2});
    background->set_repositioning(ecs::abstractions::gui::drawable_element::reposition_center);
    game.emplace_component<ecs::components::gui::drawable>(button, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), std::move(background)},
            {static_cast<ecs::entity>(game), std::move(label)}
        })
    });

    game.run();

    return 0;
}
