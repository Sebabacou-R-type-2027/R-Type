/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MapEditorState
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <networkClient/Client.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include "game_state.hpp"
#include "registry.hpp"
#include "system.hpp"
#include "../factories/button_factory.hpp"
#include "../game/GamePlayState.hpp"
#include "../utils/Settings.hpp"

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


namespace rtype {
    struct Mob {
        float x, y;
        std::string type;
    };

    class Wave {
    public:
        int wave_id;
        std::vector<Mob> mobs;
        std::vector<ecs::Entity> entityIds;

        Wave(int id) : wave_id(id) {}

        void addMob(float x, float y, const std::string& type) {
            mobs.push_back(Mob{x, y, type});
        }

        void addEntityId(ecs::Entity entityId) {
            entityIds.push_back(entityId);
        }
    };

    class MapEditorState : public GameState {
    public:
        MapEditorState(sf::RenderWindow& window);

        void handleInput() override;
        void update() override;
        void render() override;

        static constexpr float GRID_SIZE = 32.0f;

    private:
        sf::RenderWindow& window;
        ecs::Registry registry;
        ecs::System system;
        sf::Font font;
        sf::Shader backgroundShader;
        sf::Texture enemyTexture;
        nlohmann::json map;
        std::vector<Wave> waves;

        int currentWave;
        int previousWave;

        sf::Vector2f mousePosWorld;
        sf::Vector2f mousePosView;

        void displayWave();
        std::vector<Mob>& getMobs(int waveId);
        bool switchWave(int previousWave, int newWave);
        void saveMap();
        void loadMap();
        void saveWavesToJson();
        void loadWavesFromJson();

        void renderGrid();
        void renderPreview();
    };
}
