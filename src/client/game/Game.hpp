/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "game_state.hpp"
#include <memory>
#include <networkClient/Client.hpp>

#include "../ecs/registry.hpp"
#include "../ecs/system.hpp"

namespace rtype {
    class GameState;

    /**
     * @brief Class that contains the game
     */
    class Game {
    public:
        /**
         * @brief Construct a new Game object
         *
         * @param title contains the title of the game
         * @param width contains the width of the window
         * @param height contains the height of the window
         */
        Game(const std::string& title, unsigned int width, unsigned int height, client::Client& network);
        /**
         * @brief Run the game
         *
         * @return void
         */
        void run();
        /**
         * @brief Change the state of the game
         *
         * @param newState contains the new state of the game
         * @return void
         */
        void changeState(std::shared_ptr<GameState> newState);

        /**
         * @brief Get the window of the game
         *
         * @return sf::RenderWindow&
         */
        ecs::Registry& getRegistry() { return registry; }
        /**
         * @brief Get the current state of the game
         *
         * @return std::shared_ptr<GameState>
         */
        ecs::System& getSystem() { return system; }

        /**
         * @brief Get the window of the game
         *
         * @return sf::RenderWindow&
         */
        sf::Shader& getBackgroundShader() { return backgroundShader; }

    private:
        sf::RenderWindow window; ///< Window of the game
        std::shared_ptr<GameState> currentState; ///< Current state of the game
        sf::Shader backgroundShader; ///< Shader for the background
        ecs::SoundEffect Sound_game; ///< Sound of the game

        client::Client& network_;
        ecs::Registry registry; ///< Registry to store all the components
        ecs::System system; ///< System to store all the systems
    };
}
