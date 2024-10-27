#ifndef POWERUP_HPP
#define POWERUP_HPP

#include <SFML/Graphics.hpp>
#include "../game_state.hpp"
#include "../ecs/registry.hpp"
#include "../utils/Settings.hpp"
#include <iostream>
#include "../game/Game.hpp"
#include "../game/GamePlayState.hpp"
#include "../factories/button_factory.hpp"
namespace rtype {
    class PowerUp : public GameState {
    public:
        PowerUp(ecs::Registry& registry, sf::RenderWindow& window);

        void handleInput() override;
        void render() override;
        void update() override;

    private:
        ecs::Registry& registry;
        sf::RenderWindow& window;
        sf::Shader backgroundShader; // Ou le type approprié
        bool isCreate = false;
        sf::Font font;
        ecs::System system;
    };
}

#endif // POWERUP_HPP
