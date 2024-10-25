/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Handle_collision
*/

#include "Handle_collision.hpp"
#include "components/Entity_type.hpp"
#include <iostream>


namespace ecs {

    void HandleCollision::create_explosion(Registry& registry, float x, float y) {
        auto explosion = registry.spawn_entity();
        auto &animation = registry.emplace_component<Animation>(explosion, "assets/sprites/r-typesheet3.gif", 12, 1, 0.1f);
        registry.emplace_component<Position>(explosion, x, y);
        registry.emplace_component<LifeState>(explosion, true);
    }

    void HandleCollision::solo_handle_collision(std::size_t i, Registry& registry) {
        auto &collisionstates = registry.get_components<CollisionState>();
        auto &entitytypes = registry.get_components<EntityType>();
        auto &positions = registry.get_components<Position>();
        auto &lifestates = registry.get_components<LifeState>();
        auto &scores = registry.get_components<Score>();
        auto &killlogs = registry.get_components<KillLog>();
        auto players = registry.get_all_player_entity();

        for (auto playerEntity : players) {
            auto& killlog = killlogs[static_cast<std::size_t>(playerEntity)];
            for (std::size_t j = 0; j < collisionstates.size() && j < entitytypes.size() && j < positions.size() && j < lifestates.size() && j < scores.size(); ++j) {
                if (collisionstates[j] && collisionstates[j]->active
                    && i < entitytypes.size() && j < entitytypes.size()
                    && entitytypes[i]->current_type != entitytypes[j]->current_type
                    && lifestates[i]->isAlive && lifestates[j]->isAlive) {
                        if (scores[i] && killlog) {
                            killlog->addKill(j);
                        } else if (scores[j] && killlog) {
                            killlog->addKill(i);
                        }
                        lifestates[i]->isAlive = false;
                        positions[i]->x = -1000;
                        positions[i]->y = -1000;

                        lifestates[j]->isAlive = false;
                        positions[j]->x = -1000;
                        positions[j]->y = -1000;
                        continue;
                    }
            }
        }
    }

    void HandleCollision::network_handle_collision(std::size_t i, Registry& registry, client::Client& network) {
        auto &collisionstates = registry.get_components<CollisionState>();
        auto &entitytypes = registry.get_components<EntityType>();
        auto &positions = registry.get_components<Position>();
        auto &lifestates = registry.get_components<LifeState>();
        auto &scores = registry.get_components<Score>();
        auto &killlogs = registry.get_components<KillLog>();
        auto players = registry.get_all_player_entity();

        if (i != network.my_id_in_lobby_) {
            return;
        }
        for (auto playerEntity : players) {
            auto& killlog = killlogs[static_cast<std::size_t>(playerEntity)];
            for (std::size_t j = 0; j < collisionstates.size() && j < entitytypes.size() && j < positions.size() && j < lifestates.size() && j < scores.size(); ++j) {
                if (collisionstates[j] && collisionstates[j]->active
                    && i < entitytypes.size() && j < entitytypes.size()
                    && entitytypes[i]->current_type != entitytypes[j]->current_type
                    && lifestates[i]->isAlive && lifestates[j]->isAlive) {
                        if (scores[i] && killlog) {
                            killlog->addKill(j);
                        } else if (scores[j] && killlog) {
                            killlog->addKill(i);
                        }
                        std::cout << "kill log = " << killlog->killedEnemies.size() << std::endl;
                        lifestates[i]->isAlive = false;
                        positions[i]->x = -1000;
                        positions[i]->y = -1000;

                        lifestates[j]->isAlive = false;
                        positions[j]->x = -1000;
                        positions[j]->y = -1000;
                        continue;
                    }
            }
        }
    }
    void HandleCollision::handle_collision(Registry& registry, client::Client& network, bool isSolo) {
        auto &collisionstates = registry.get_components<CollisionState>();
        auto &entitytypes = registry.get_components<EntityType>();
        auto &positions = registry.get_components<Position>();
        auto &lifestates = registry.get_components<LifeState>();
        auto &scores = registry.get_components<Score>();

        for (std::size_t i = 0; i < entitytypes.size() && i < collisionstates.size() && i < positions.size() && i < lifestates.size(); ++i) {
                if (isSolo && collisionstates[i] && collisionstates[i]->active) {
                    solo_handle_collision(i, registry);
                } else if (!isSolo && collisionstates[i] && collisionstates[i]->active) {
                    network_handle_collision(i, registry, network);
                }
            }
        }
}
