/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** registry
*/

#pragma once

            #include <unordered_map>
            #include <vector>
            #include <typeindex>
            #include <any>
            #include <algorithm>
            #include "sparse_array.hpp"
            #include "entity.hpp"

            // include all components
            #include "components/Position.hpp"
            #include "components/Velocity.hpp"
            #include "components/Controllable.hpp"
            #include "components/Drawable.hpp"
            #include "components/LoopMovement.hpp"
            #include "components/Hitbox.hpp"
            #include "components/Button.hpp"
            #include "components/Animation.hpp"
            #include "components/Entity_type.hpp"
            #include "components/Bullet.hpp"
            #include "components/Collision_state.hpp"
            #include "components/GameState.hpp"
            #include "components/LifeState.hpp"
            #include "components/BulletCharge.hpp"
            #include "components/Shooting.hpp"
            #include "components/Chasing.hpp"
            #include "components/Spawner.hpp"
            #include "components/Score.hpp"
            #include "components/EnemyLog.hpp"
            #include "components/ScoreText.hpp"

namespace ecs {

class Registry {
    /**
     * @brief Class that contains all the components
     * 
     * @tparam Component
     */
    public:

        /**
         * @brief Register a component in the registry
         * 
         * @tparam Component
         * @return sparse_array<Component>& Reference to the component array
         */
        template <typename Component>
        sparse_array<Component>& register_component() {
            auto type = std::type_index(typeid(Component));
            _components[type] = sparse_array<Component>();
            return std::any_cast<sparse_array<Component>&>(_components[type]);
        }

        /**
         * @brief Get the components of a specific type
         * 
         * @tparam Component
         * @return sparse_array<Component>& Reference to the component array
         */
        template <typename Component>
        sparse_array<Component>& get_components() {
            auto type = std::type_index(typeid(Component));
            return std::any_cast<sparse_array<Component>&>(_components.at(type));
        }


        /**
         * @brief Get the components of a specific type
         * 
         * @tparam Component
         * @return const sparse_array<Component>& Reference to the component array
         */
        template <typename Component>
        const sparse_array<Component>& get_components() const {
            auto type = std::type_index(typeid(Component));
            return std::any_cast<const sparse_array<Component>&>(_components.at(type));
        }

        /**
         * @brief Get the number of entities
         * 
         * @return std::size_t Number of entities
         */
        Entity spawn_entity() {
            Entity new_entity(_next_entity_id++);
            _entities.push_back(new_entity);
            return new_entity;
        }

        std::size_t entity_count() const {
            return _entities.size();
        }
        Entity entity_from_index(std::size_t idx) const {
            return _entities.at(idx);
        }

        /**
         * @brief Kill an entity
         * 
         * @param e Entity to kill
         */
        void kill_entity(Entity const& e) {
            for (auto& [type, component_array] : _components) {
                try {
                    auto& sparse_array_ref = std::any_cast<sparse_array<std::optional<std::remove_reference_t<decltype(component_array)>>> &>(component_array);
                    sparse_array_ref.erase(static_cast<std::size_t>(e));
                } catch (const std::bad_any_cast&) {

                }
            }
            _entities.erase(std::remove(_entities.begin(), _entities.end(), e), _entities.end());
        }

        /**
         * @brief Add a component to an entity
         * 
         * @tparam Component
         * @param to Entity to add the component to
         * @param component Component to add
         * @return typename sparse_array<Component>::reference_type Reference to the added component
         */
        template <typename Component>
        typename sparse_array<Component>::reference_type add_component(Entity const& to, Component&& component) {
            return get_components<Component>().insert_at(static_cast<std::size_t>(to), std::forward<Component>(component));
        }

        /**
         * @brief Add a component to an entity
         * 
         * @tparam Component
         * @param to Entity to add the component to
         * @param component Component to add
         * @return typename sparse_array<Component>::reference_type Reference to the added component
         */
        template <typename Component, typename... Params>
        typename sparse_array<Component>::reference_type emplace_component(Entity const& to, Params&&... params) {
            auto& component = get_components<Component>().emplace_at(static_cast<std::size_t>(to), std::forward<Params>(params)...);
            return component;
    }


        template <typename Component>
        void remove_component(Entity const& from) {
            get_components<Component>().erase(static_cast<std::size_t>(from));
        }

        Entity get_player_entity() const {
            const auto& types = get_components<EntityType>();
            for (std::size_t i = 0; i < types.size(); ++i) {
                if (types[i] && types[i]->current_type == Type::Player) {
                    return entity_from_index(i);
                }
            }
            throw std::runtime_error("Player entity not found");
        }

        std::vector<Entity> get_all_player_entity() const {
            std::vector<Entity> playerEntities;
            const auto& types = get_components<EntityType>();
            for (std::size_t i = 0; i < types.size(); ++i) {
                if (types[i] && types[i]->current_type == Type::Player) {
                    playerEntities.push_back(entity_from_index(i));
                }
            }
            return playerEntities;
        }

        std::vector<Entity> get_all_enemy_entity() const {
            std::vector<Entity> enemyEntities;
            const auto& types = get_components<EntityType>();
            for (std::size_t i = 0; i < types.size(); ++i) {
                if (types[i] && types[i]->current_type == Type::Ennemy) {
                    enemyEntities.push_back(entity_from_index(i));
                }
            }
            return enemyEntities;
        }

        bool has_component(Entity const& e, std::type_index type) const {
            if (_components.find(type) == _components.end()) {
            return false;
            }
            const auto& component_array = std::any_cast<const sparse_array<std::optional<std::remove_reference_t<decltype(_components.at(type))>>> &>(_components.at(type));
            return component_array.size() > static_cast<std::size_t>(e) && component_array[static_cast<std::size_t>(e)].has_value();
        }

        std::pair<std::type_index, std::optional<std::any>> get_component(Entity const& e, std::type_index type) const {
            for (const auto& [type, component_array] : _components) { // Note the `const` on component_array
                try {
                    // Cast component_array as a const sparse_array with std::optional<std::any>
                    const auto& sparse_array_ref = std::any_cast<const sparse_array<std::optional<std::any>>&>(component_array);
                    if (sparse_array_ref.size() > static_cast<std::size_t>(e) && sparse_array_ref[static_cast<std::size_t>(e)].has_value()) {
                        return std::make_pair(type, sparse_array_ref[static_cast<std::size_t>(e)]);
                    }
                } catch (const std::bad_any_cast&) {
                    // Handle the casting error if component_array isn't of expected type
                }
            }
            return std::make_pair(std::type_index(typeid(void)), std::nullopt);
        }
        void register_all_components() {
            register_component<Position>();
            register_component<Velocity>();
            register_component<Controllable>();
            register_component<Drawable>();
            register_component<LoopMovement>();
            register_component<Hitbox>();
            register_component<Button>();
            register_component<Animation>();
            register_component<EntityType>();
            register_component<Bullet>();
            register_component<CollisionState>();
            register_component<GameStateComponent>();
            register_component<LifeState>();
            register_component<BulletCharge>();
            register_component<Shooting>();
            register_component<Chasing>();
            register_component<Spawner>();
            register_component<Score>();
            register_component<KillLog>();
            register_component<ScoreText>();
        }
    private:
        std::size_t _next_entity_id = 0;
        std::vector<Entity> _entities;

        std::unordered_map<std::type_index, std::any> _components;
};

}