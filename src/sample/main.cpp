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
    const ecs::components::gui::asset_manager &asset_manager = *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.add_component(e, enemy{1, 1, 100, std::chrono::steady_clock::now()});
    game.add_component(e, ecs::components::position{position.x, position.y});
    game.add_component(e, ecs::components::engine::velocity{-10.0f, 0.0f});
    game.emplace_component<health>(e, 1, game);
    game.add_component(e, ecs::components::lifestate{});
    game.add_component(e, ecs::components::engine::hitbox{34.0f, 36.0f, position.x, position.x});
    game.add_component(e, enemy_loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 1.0f, 0.0f, 100.0f, 2.0f});
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::animation>(
                asset_manager.get_texture("enemy"), 1, 8, 200ms, "enemy")
            }
        })
    });
}

static void spawn_enemy_chaser(ecs::entity target, const ecs::components::position& position, game::game &game)
{
    const ecs::components::gui::asset_manager &asset_manager = *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.add_component(e, enemy{1, 1, 300, std::chrono::steady_clock::now()});
    game.add_component(e, ecs::components::position{position.x, position.y});
    game.add_component(e, ecs::components::lifestate{});
    game.emplace_component<health>(e, 1, game);
    game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
    game.add_component(e, ecs::components::engine::hitbox{33.0f, 34.0f, 0.0f, 0.0f});
    game.add_component(e, enemy_chaser{target, 10.0f});
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::animation>
                (asset_manager.get_texture("enemy_chaser"), 1, 3, 200ms, "enemy_chaser")
            }
        })
    });
}

static void spawn_enemy_spawner(const ecs::components::position& position, game::game &game)
{
    const auto now = std::chrono::steady_clock::now();
    const ecs::components::gui::asset_manager &asset_manager = *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.emplace_component<enemy>(e, 2, 0, 500, now);
    game.emplace_component<enemy_spawner>(e, 2s, 5ul, game);
    game.add_component(e, ecs::components::lifestate{});
    game.emplace_component<health>(e, 3, game);
    game.emplace_component<ecs::components::position>(e, position.x, position.y);
    game.emplace_component<ecs::components::engine::hitbox>(e, 65.2f, 66.0f, 0.0f, 0.0f);
    game.emplace_component<ecs::components::engine::velocity>(e);
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::animation>
                (asset_manager.get_texture("enemy_spawner"), 1, 5, 200ms, "enemy_spawner")
            }
        })
    });
}

static void spawn_enemy_shooter(const ecs::components::position& position, game::game &game)
{
    const ecs::components::gui::asset_manager &asset_manager = *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto windowSize = game.get_entity_component<ecs::components::gui::window>(game)->get().window->getSize();
    auto e = game.create_entity();
    game.add_component(e, enemy{1, 1, 250, std::chrono::steady_clock::now()});
    game.add_component(e, ecs::components::position{position.x, position.y});
    game.emplace_component<health>(e, 1, game);
    game.add_component(e, ecs::components::lifestate{});
    game.add_component(e, ecs::components::engine::velocity{0.0f, 10.0f});
    game.add_component(e, ecs::components::engine::hitbox{65.0f, 50.0f, 0.0f, 0.0f});
    game.add_component(e, projectile_launcher{-1.0, 1s, std::chrono::steady_clock::now(), game});
    game.add_component(e, enemy_shooter{game});
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::animation>
                (asset_manager.get_texture("enemy_shooter"), 1, 3, 200ms, "enemy_shooter")
            }
        })
    });
}


static void initializeEnemies(game::game &game) noexcept {
    spawn_enemy({800.0f, 500.0f}, game);
    spawn_enemy_chaser(game.player, {800.0f, 400.0f}, game);
    spawn_enemy_spawner({300.0f, 300.0f}, game);
    spawn_enemy_shooter({400.0f, 400.0f}, game);
}

int main()
{
    game::game game;
    game.begin_scene(std::make_unique<game::scenes::menu>(game));

    game.run();

    return 0;
}
