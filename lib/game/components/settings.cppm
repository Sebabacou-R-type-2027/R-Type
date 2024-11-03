#if __cpp_lib_modules < 202207L
module;

#include <functional>
#endif
export module game:components.settings;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

using namespace ecs::abstractions;

export namespace game::components {
    struct settings {
        std::string username = "";
        std::string password = "";
        std::string server_address = "";
        std::string port = "";

        bool isShaderEnabled = true;
    };
}