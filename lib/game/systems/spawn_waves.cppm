module;

#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

export module game:systems.spawn_waves;
import :components.enemies;
import :components.projectiles;
import :components.stats;
import :components.spawn_waves;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

using json = nlohmann::json;

export namespace game::systems {
    void spawn_mob(ecs::entity_container &ec, components::spawn_waves &waves, ecs::components::position position)
    {
        auto enemy = ec.create_entity();

        ec.add_component(enemy, position);
        ec.add_component(enemy, ecs::components::engine::velocity{-10.0f, 0.0f});
        ec.add_component(enemy, components::health{1, waves.game});
        ec.add_component(enemy, ecs::components::engine::hitbox{ecs::abstractions::rectangle<float>{500.0f, 500.0f, 34.0f, 36.0f}});
        ec.add_component(enemy, components::enemy_loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 1.0f, 0.0f, 100.0f, 2.0f, waves.game});
        ec.emplace_component<ecs::components::gui::drawable>(enemy, ecs::components::gui::drawable{waves.game,
            std::container<ecs::components::gui::drawable::elements_container>::make({
                {static_cast<ecs::entity>(waves.game), ec.get_entity_component<ecs::components::gui::display>(waves.game)->get().factory->make_element(
                    "Enemy", ec.get_entity_component<ecs::components::gui::asset_manager>(waves.game)->get().get("arial"), 12)},
                {static_cast<ecs::entity>(waves.game), ec.get_entity_component<ecs::components::gui::display>(waves.game)->get().factory->make_element(
                    dynamic_cast<const ecs::abstractions::gui::texture &>(ec.get_entity_component<ecs::components::gui::asset_manager>(waves.game)->get().get("enemy")), {8, 1}, 10ms)}
            })
        });
    }

    void spawn_mob_type(ecs::entity_container &ec, components::spawn_waves &waves, ecs::components::position position, std::string mob_type)
    {
        if (mob_type == "enemy")
            spawn_mob(ec, waves, position);
    }

    void spawn_wave(ecs::entity_container &ec, components::spawn_waves &waves_component) noexcept
    {
        std::ifstream file("waves.json");
        if (!file.is_open())
            return;
        nlohmann::json waves;
        file >> waves;

        std::cout <<waves[std::to_string(waves_component.current_wave)] << std::endl;

        for (auto &mob : waves[std::to_string(waves_component.current_wave)])
        {
            ecs::components::position position{0.0f, 0.0f};
            position.x = mob["x"];
            position.y = mob["y"];

            spawn_mob_type(ec, waves_component, position, mob["type"]);
            std::cout << "Spawned enemy at " << position.x << ", " << position.y << std::endl;
        }
    }
    void spawn_waves_mobs(ecs::entity_container &ec, components::spawn_waves &waves) noexcept
    {
        if (waves.current_wave > 10)
            return;
        // std::cout << "Current wave: " << waves.current_wave << std::endl;
        auto now = std::chrono::steady_clock::now();
        static std::chrono::steady_clock::time_point last_spawn = now;
        if (now - last_spawn < 5s)
            return;
        last_spawn = now;
        spawn_wave(ec, waves);
        waves.current_wave++;
    }
}