export module game:components.settings;

import std;
import ecs;

using namespace ecs::abstractions;

export namespace game::components {

    /**
        * @brief Component that defines the settings
        *
        * This component is used to define the settings of the game. It contains the username, password, server address, port, and shader status.
     */
    struct settings {
        std::string username = "";
        std::string password = "";
        std::string server_address = "";
        std::string port = "";

        bool isShaderEnabled = true;
    };
}
