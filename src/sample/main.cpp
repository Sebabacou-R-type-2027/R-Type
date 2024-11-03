#if __cpp_lib_modules < 202207L
module;
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <asio/io_context.hpp>
#endif

#include <asio.hpp>

#if __cpp_lib_modules >= 202207L
import std;
#endif

import ecs;
import utils;
import game;
import Client;

using namespace ecs;
using namespace game::components;

using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
        return 1;
    }
    asio::io_context io_context;
    Client client(io_context);
    game::game game;

    try {
        std::thread network_thread(&Client::main_loop, &client);
        client.connect(argv[1], std::stoi(argv[2]));
        game.begin_scene(std::make_unique<game::scenes::menu>(game));
        game.run();
        network_thread.join();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}


