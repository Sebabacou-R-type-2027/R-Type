/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ennemy_state_system
*/

#include "game_state_manager.hpp"

namespace ecs::systems {

    void GameStateManager::update(Registry& registry, float deltaTime) {
        auto states = registry.get_components<GameStateComponent>();

        for (std::size_t i = 0; i < states.size(); ++i) {
            if (states[i]) {
                GameStateComponent &state = *states[i];

                switch (state.current_state) {
                    case GameState::MENU:
                        break;

                    case GameState::GAME:
                        break;

                    case GameState::PAUSE:
                        break;

                    case GameState::GAMEOVER:
                        break;
                }
            }
        }
    }
}
