#if __cpp_lib_modules < 202207L
module;

#include <functional>
#include <string>
#include <vector>
#include <map>
#endif
export module game:components.map_editor;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

using namespace ecs::abstractions;

export namespace game::components {
    struct map_editor {
        int selectedWave = 1;
        std::string selectedMob = "enemy";
        std::vector<std::string> mobTypes = {"enemy", "enemy_chaser"};

        std::map<int, std::vector<std::pair<std::string, ecs::abstractions::vector<float>>>> waves = {
        };

        ecs::entity game;
    };
}