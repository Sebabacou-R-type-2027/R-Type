export module game:components.stats;

import std;
import ecs;

export namespace game::components {

    /**
        * @brief Component that defines the score

        * This component is used to define the score of the game. It contains the value and the game entity.
     */
    struct score {
        int value;
        ecs::entity game;

        score(int value, ecs::entity game)
            : value(value), game(game)
        {}
    };

    /**
        * @brief Component that defines the health

        * This component is used to define the health of the entity. It contains the value and the game entity.
     */
    struct health {
        int value;
        ecs::entity game;
    };
}
