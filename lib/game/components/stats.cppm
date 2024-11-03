#if __cpp_lib_modules < 202207L
module;

#endif
export module game:components.stats;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

export namespace game::components {
    struct score {
        int value;
        ecs::entity game;

        score(int value, ecs::entity game)
            : value(value), game(game)
        {}
    };

    struct health {
        int value;
        ecs::entity game;
    };
}
