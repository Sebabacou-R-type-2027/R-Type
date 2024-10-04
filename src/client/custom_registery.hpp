/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** custom_registery
*/

#pragma once

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <any>
#include <algorithm>
#include "../ecs/sparse_array.hpp"
#include "../ecs/entity.hpp"

namespace rtype {

class CustomRegistry {
    public:
        template <typename Component>
        ecs::sparse_array<Component>& register_component() {
            auto type = std::type_index(typeid(Component));
            _components[type] = sparse_array<Component>();
            return std::any_cast<sparse_array<Component>&>(_components[type]);
        }

        template <typename Component>
        ecs::sparse_array<Component>& get_components() {
            auto type = std::type_index(typeid(Component));
            return std::any_cast<sparse_array<Component>&>(_components.at(type));
        }

        template <typename Component>
        const sparse_array<Component>& get_components() const {
            auto type = std::type_index(typeid(Component));
            return std::any_cast<const sparse_array<Component>&>(_components.at(type));
        }

        ecs::Entity spawn_entity() {
            ecs::Entity new_entity(_next_entity_id++);
            _entities.push_back(new_entity);
            return new_entity;
        }

        ecs::Entity entity_from_index(std::size_t idx) const {
            return _entities.at(idx);
        }

        void kill_entity(ecs::Entity const& e) {
            for (auto& [type, component_array] : _components) {
                try {
                    auto& sparse_array_ref = std::any_cast<ecs::sparse_array<std::optional<std::remove_reference_t<decltype(component_array)>>> &>(component_array);
                    sparse_array_ref.erase(static_cast<std::size_t>(e));
                } catch (const std::bad_any_cast&) {

                }
            }
            _entities.erase(std::remove(_entities.begin(), _entities.end(), e), _entities.end());
        }

        template <typename Component>
        typename ecs::sparse_array<Component>::reference_type add_component(ecs::Entity const& to, Component&& component) {
            return get_components<Component>().insert_at(static_cast<std::size_t>(to), std::forward<Component>(component));
        }

        template <typename Component, typename... Params>
        typename ecs::sparse_array<Component>::reference_type emplace_component(ecs::Entity const& to, Params&&... params) {
            return get_components<Component>().emplace_at(static_cast<std::size_t>(to), std::forward<Params>(params)...);
        }


        template <typename Component>
        void remove_component(ecs::Entity const& from) {
            get_components<Component>().erase(static_cast<std::size_t>(from));
        }

        void register_all_components() {
            
        }
    private:
        std::size_t _next_entity_id = 0;
        std::vector<ecs::Entity> _entities;

        std::unordered_map<std::type_index, std::any> _components;
};

}