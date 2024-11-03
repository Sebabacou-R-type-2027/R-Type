module;

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
export module game:systems.map_editor;
import :components.map_editor;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

using json = nlohmann::json;
using namespace ecs;

bool is_position_valid(ecs::abstractions::vector<float> position) noexcept
{
    if (position.x < 0 || position.y < 0 || position.x > 1920 || position.y > 1080)
        return false;
    return true;
}

export namespace game::systems {
    void handle_map_editor(entity_container &ec, components::map_editor &map_editor, const ecs::components::gui::display &display) noexcept
    {
        // render the current wave
        const ecs::components::gui::asset_manager &asset_manager = *ec.get_entity_component<const ecs::components::gui::asset_manager>(map_editor.game);


        auto text = display.factory->make_element("Wave: " + std::to_string(map_editor.selectedWave), asset_manager.get("arial"), 24);
        text->position({10.0f, 10.0f});
        display.window->draw(*text);

        auto animation = display.factory->make_element(dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("enemy")), {8, 1});

        for (auto &wave : map_editor.waves)
        {
            if (wave.first != map_editor.selectedWave)
                continue;
            for (auto &mob : wave.second)
            {
                if (mob.first == "enemy")
                    animation = display.factory->make_element(dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("enemy")), {8, 1});
                else if (mob.first == "enemy_chaser")
                    animation = display.factory->make_element(dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("enemy_chaser")), {3, 1});
                else if (mob.first == "enemy_shooter")
                    animation = display.factory->make_element(dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("enemy_shooter")), {3, 1});
                else if (mob.first == "enemy_spawner")
                    animation = display.factory->make_element(dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("enemy_spawner")), {5, 1});
                else if (mob.first == "boss-phase")
                    animation = display.factory->make_element(dynamic_cast<const ecs::abstractions::gui::texture &>(asset_manager.get("boss-phase")), {4, 1});

                animation->position({mob.second.x, mob.second.y});
                animation.get()->update(std::chrono::steady_clock::now().time_since_epoch());

                display.window->draw(*animation);
            }
        }
        if (display.window->is_input_active(ecs::abstractions::gui::inputs::lctrl) && display.window->is_input_active(ecs::abstractions::gui::inputs::s))
        {

            std::cout << "Saving waves" << std::endl;
            json save_waves;

            for (auto &wave : map_editor.waves)
            {
                json wave_json;
                for (auto &mob : wave.second)
                {
                    json mob_json;
                    mob_json["type"] = mob.first;
                    mob_json["x"] = mob.second.x;
                    mob_json["y"] = mob.second.y;
                    wave_json.push_back(mob_json);
                }
                save_waves[std::to_string(wave.first)] = wave_json;
            }

            std::ofstream file("assets/waves.json");

            file << save_waves.dump(4);
            file.close();
        }

        if (display.window->is_input_active(ecs::abstractions::gui::inputs::left)) {
            if (map_editor.selectedWave == 1)
                map_editor.selectedWave = map_editor.waves.size();
            else
                map_editor.selectedWave--;
        }

        if (display.window->is_input_active(ecs::abstractions::gui::inputs::right)) {
            if (map_editor.selectedWave == map_editor.waves.size())
                map_editor.selectedWave = 1;
            else
                map_editor.selectedWave++;
        }

        if (display.window->is_input_active(ecs::abstractions::gui::inputs::one) || display.window->is_input_active(ecs::abstractions::gui::inputs::numpad1))
            map_editor.selectedMob = map_editor.mobTypes[0];
        if (display.window->is_input_active(ecs::abstractions::gui::inputs::two) || display.window->is_input_active(ecs::abstractions::gui::inputs::numpad2))
            map_editor.selectedMob = map_editor.mobTypes[1];
        if (display.window->is_input_active(ecs::abstractions::gui::inputs::three) || display.window->is_input_active(ecs::abstractions::gui::inputs::numpad3))
            map_editor.selectedMob = map_editor.mobTypes[2];
        if (display.window->is_input_active(ecs::abstractions::gui::inputs::four) || display.window->is_input_active(ecs::abstractions::gui::inputs::numpad4))
            map_editor.selectedMob = map_editor.mobTypes[3];
        if (display.window->is_input_active(ecs::abstractions::gui::inputs::five) || display.window->is_input_active(ecs::abstractions::gui::inputs::numpad5))
            map_editor.selectedMob = map_editor.mobTypes[4];

        if (display.window->is_input_active(ecs::abstractions::gui::inputs::lclick)) {
            auto position = display.window->get_cursor_position();
            if (is_position_valid(position)) {
                position.x = std::floor(position.x / 32.0f) * 32.0f;
                position.y = std::floor(position.y / 32.0f) * 32.0f;
                // check if the position is already taken then delete the mob
                for (auto &wave : map_editor.waves)
                {
                    for (auto it = wave.second.begin(); it != wave.second.end(); ++it)
                    {
                        if (it->second.x == position.x && it->second.y == position.y)
                        {
                            wave.second.erase(it);
                            break;
                        }
                    }
                }
                map_editor.waves[map_editor.selectedWave].push_back({map_editor.selectedMob , {static_cast<float>(position.x), static_cast<float>(position.y)}});
            }

        }

        if (display.window->is_input_active(ecs::abstractions::gui::inputs::rclick)) {
            auto position = display.window->get_cursor_position();
            if (is_position_valid(position)) {
                position.x = std::floor(position.x / 32.0f) * 32.0f;
                position.y = std::floor(position.y / 32.0f) * 32.0f;
                for (auto &wave : map_editor.waves)
                {
                    for (auto it = wave.second.begin(); it != wave.second.end(); ++it)
                    {
                        if (it->second.x == position.x && it->second.y == position.y)
                        {
                            wave.second.erase(it);
                            break;
                        }
                    }
                }
            }
        }


        if (display.window->is_input_active(ecs::abstractions::gui::inputs::space)) {
            map_editor.waves[map_editor.selectedWave].clear();
        }

    }
}