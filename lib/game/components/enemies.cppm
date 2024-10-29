#if __cpp_lib_modules < 202207L
module;

#include <chrono>
#endif
export module game:components.enemies; 
import :components.projectiles;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

using namespace std::chrono_literals;

export namespace game::components {
    /**
        * @brief Component that defines the enemies

        * This component is used to define the enemies of the game. It contains the health and damage of the entity.
     */
    struct enemy {
        int health; ///< Health of the entity
        int damage; ///< Damage of the entity
        bool isAlive = true;
        std::chrono::steady_clock::time_point birth;

        /**
            * @brief Constructor of the enemy component

            * @param health Health of the entity
            * @param damage Damage of the entity
         */
        enemy(int health, int damage, std::chrono::steady_clock::time_point birth)
            : health(health), damage(damage), birth(birth)
        {}
    };

    struct enemy_ownership {
        ecs::entity owner;
    };

    /**
        * @brief Component that defines the enemy chaser

        * This component is used to define the enemy chaser. It contains the target entity, speed of the entity.
     */
    struct enemy_chaser {
        float _speed; ///< Speed of the entity
        ecs::entity _target; ///< Target entity

        /**
            * @brief Constructor of the enemy chaser component

            * @param target Target entity
            * @param speed Speed of the entity
         */
        enemy_chaser(ecs::entity target, float speed)
            : _speed(speed), _target(target) {}
    };

    /**
        * @brief Component that defines the enemy shooter

        * This component is used to define the enemy shooter. It contains the cooldown between shots and the last time the entity shot.
     */
    struct enemy_shooter {
        bool moving_up = true;
        float speed = 5.0f;
        ecs::entity game;

        /**
            * @brief Constructor of the enemy shooter component

            * @param cooldown Cooldown between shots
         */
        enemy_shooter(ecs::entity game)
            : game(game)
        {}
    };

    /**
        * @brief Component that defines the enemy spawner

        * This component is used to define the enemy spawner. It contains the cooldown between spawns, the maximum number of enemies and the last time the entity spawned an enemy.
     */
    struct enemy_spawner {
        std::chrono::steady_clock::duration cooldown; ///< Cooldown between spawns
        std::size_t max_enemies; ///< Maximum number of enemies
        std::chrono::steady_clock::time_point last_update; ///< Last time the entity spawned an enemy
        ecs::entity game;

        /**
            * @brief Constructor of the enemy spawner component

            * @param cooldown Cooldown between spawns
            * @param max_enemies Maximum number of enemies
         */
        enemy_spawner(std::chrono::steady_clock::duration cooldown, std::size_t max_enemies, ecs::entity game)
            : cooldown(cooldown), max_enemies(max_enemies),
            last_update(std::chrono::steady_clock::time_point(0s)), game(game)
        {}
    };

    /**
        * @brief Component that defines the enemy loop movement

        * This component is used to define the enemy loop movement. It contains the min and max position on the x and y axis, the speed of the entity, the angle of the entity, the radius of the entity and the angular speed of the entity.
     */
    struct enemy_loop_movement {
        float min_x, max_x; ///< Min and max position on the x axis
        float min_y, max_y; ///< Min and max position on the y axis
        float speed; ///< Speed of the entity
        float angle; ///< Angle of the entity
        float radius; ///< Radius of the entity
        float angular_speed; ///< Angular speed of the entity

        /**
            * @brief Constructor of the enemy loop movement component

            * @param min_x Min position on the x axis
            * @param max_x Max position on the x axis
            * @param min_y Min position on the y axis
            * @param max_y Max position on the y axis
            * @param speed Speed of the entity
            * @param angle Angle of the entity
            * @param radius Radius of the entity
            * @param angular_speed Angular speed of the entity
         */
        enemy_loop_movement(float min_x, float max_x, float min_y, float max_y, float speed, float angle, float radius, float angular_speed)
            : min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y), speed(speed), angle(angle), radius(radius), angular_speed(angular_speed) {}
    };
        
        
}
