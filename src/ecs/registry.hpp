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
#include "components/Button.hpp"
#include "components/Hitbox.hpp"
#include "components/Animation.hpp"

#include "components/Ennemy_state.hpp"
#include "components/Entity_type.hpp"
#include "components/Bullet.hpp"
#include "components/Collision_state.hpp"

namespace ecs {

class Registry {
    public:
        template <typename Component>
        void register_component() {
            _components.emplace(std::type_index(typeid(Component)), sparse_array<std::any>());
        }

        template <typename Component>
        auto get_components() {
            auto &components = _components.at(typeid(Component));
            std::vector<std::optional<std::reference_wrapper<Component>>> references(components.size());
            std::for_each(components.begin(), components.end(), [&references](auto& component) {
                if (component.has_value())
                    references.emplace_back(std::ref(std::any_cast<Component &>(*component)));
                else
                    references.push_back(std::nullopt);
            });
            return std::move(references);
        }

        template <typename Component>
        auto get_components() const {
            auto &components = _components.at(typeid(Component));
            std::vector<std::optional<std::reference_wrapper<const Component>>> references(components.size());
            std::for_each(components.begin(), components.end(), [&references](auto& component) {
                if (component.has_value())
                    references.push_back(std::cref(std::any_cast<const Component &>(*component)));
                else
                    references.push_back(std::nullopt);
            });
            return std::move(references);
        }

        bool entity_exists(Entity entity) const {
            return std::find(_entities.begin(), _entities.end(), entity) != _entities.end();
        }

        Entity spawn_entity() {
            auto next = std::find(_entities.begin(), _entities.end(), false);
            if (next != _entities.end()) {
                *next = true;
                return static_cast<Entity>(std::distance(_entities.begin(), next));
            }
            Entity new_entity(_next_entity_id++);
            _entities.push_back(true);
            return new_entity;
        }

        std::optional<Entity> entity_from_index(std::size_t idx) const {
            if (idx >= _entities.size())
                return std::nullopt;
            return _entities.at(idx) ? std::optional<Entity>(idx) : std::nullopt;
        }

        std::size_t size() const {
            return _entities.size();
        }

        void kill_entity(Entity entity) {
            std::cout << "Killing entity #" << static_cast<std::size_t>(entity) << std::endl;
            std::for_each(_components.begin(), _components.end(), [entity](auto& pair) {
                if (pair.second.size() > static_cast<std::size_t>(entity))
                    pair.second.erase(entity);
            });
            _entities.at(static_cast<std::size_t>(entity)) = false;
        }

        template <typename Component>
        typename sparse_array<Component>::reference_type add_component(Entity const& to, Component&& component) {
            return _components.at(typeid(Component)).insert_at(to, std::forward<Component>(component));
        }

        template <typename Component, typename... Params>
        Component &emplace_component(Entity to, Params&&... params) {
            return std::any_cast<Component &>(_components.at(typeid(Component)).emplace_at(to, Component(std::forward<Params>(params)...)).value());
        }


        template <typename Component>
        void remove_component(Entity const& from) {
            _components.at(typeid(Component)).erase(static_cast<std::size_t>(from));
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
            register_component<EnemyState>();
            register_component<EntityType>();
            register_component<Bullet>();
            register_component<CollisionState>();
        }

    private:
        std::size_t _next_entity_id = 0;
        std::vector<bool> _entities;

        std::unordered_map<std::type_index, sparse_array<std::any>> _components;
};

}
