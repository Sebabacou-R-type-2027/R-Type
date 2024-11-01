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


int main()
{
    game::game game;
    game.begin_scene(std::make_unique<game::scenes::menu>(game));

    game.run();

    return 0;
}
