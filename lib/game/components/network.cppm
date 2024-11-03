#if __cpp_lib_modules < 202207L
module;

#include <functional>
#endif
export module game:components.network;

#if __cpp_lib_modules >= 202207L
import std;

#endif
import ecs;
import Client;

using namespace ecs::abstractions;

export namespace game::components {
    struct network {
        std::shared_ptr<Client> client;
    };
}