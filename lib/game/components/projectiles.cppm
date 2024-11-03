export module game:components.projectiles;

import std;
import ecs;

export namespace game::components {

    /**
        * @brief Component that defines the projectile

        * This component is used to define the projectile of the game. It contains the damage, owner, birth time, and lifetime of the entity.
     */
    struct projectile {
        int damage;
        ecs::entity owner;
        std::chrono::steady_clock::time_point birth;
        std::chrono::steady_clock::duration lifetime;
    };


    /**
        * @brief Component that defines the projectile launcher

        * This component is used to define the projectile launcher. It contains the direction, cooldown between shots, last time the entity shot, and the game entity.
     */
    struct projectile_launcher {
        float direction;
        const std::chrono::steady_clock::duration cooldown;
        std::chrono::steady_clock::time_point last_shot;
        ecs::entity game;
    };


    /**
        * @brief Component that defines the projectile launcher ownership

        * This component is used to define the projectile launcher ownership. It contains the cooldown between shots, last time the entity shot, the game entity, and the shot status.
     */
    struct projectile_launcher_ownership {
        const std::chrono::steady_clock::duration cooldown;
        std::chrono::steady_clock::time_point last_shot;
        ecs::entity game;
        bool shot;
    };
}
