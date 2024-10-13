#pragma once

namespace rtype {
    /**
     * @brief Enum to define the state of the game
     */
    enum class GameStateType {
        MENU,
        GAME,
        PAUSE,
        GAMEOVER
    };

    /**
     * @brief Class that contains the state of the game
     */
    class GameStateManager {
    public:
        GameStateManager() : currentState(GameStateType::MENU) {}

        /**
         * @brief Get the state of the game
         * 
         * @return GameStateType State of the game
         */
        GameStateType getGameState() const {
            return currentState;
        }

        /**
         * @brief Set the state of the game
         * 
         * @param newState New state of the game
         */
        void setGameState(GameStateType newState) {
            currentState = newState;
        }

    private:
        GameStateType currentState; ///< Current state of the game
    };
}
