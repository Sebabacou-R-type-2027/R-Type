#if __cpp_lib_modules < 202207L
module;

#include <cmath>
#include <chrono>
#endif
export module game:systems.game_over;
import :components.stats;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;

using namespace std::chrono_literals;

export namespace game::systems {

    // void game_over(ecs::entity_container &ec, game &game)
    // {
    //     std::ranges::for_each(ec.get_entities(), [&ec, &game](auto e){
    //         if (auto stats = ec.get_entity_component<components::score>(e)) {
    //             return;
    //         }
    //         game.begin_scene(std::make_unique<Game_over_menu>(game));
    //     });
    // }
}
