#pragma once

namespace rtype {
    enum class GameStateType {
        MENU,
        GAME,
        PAUSE,
        GAMEOVER
    };

    class GameStateManager {
    public:
        GameStateManager() : currentState(GameStateType::MENU) {}

        GameStateType getGameState() const {
            return currentState;
        }

        void setGameState(GameStateType newState) {
            currentState = newState;
        }

    private:
        GameStateType currentState;
    };
}
