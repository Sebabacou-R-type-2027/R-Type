/*
** EPITECH PROJECT, 2024
** ecs
** File description:
** sparse_array
*/

#pragma once

#include <vector>
#include <optional>
#include <algorithm> // For std::find

namespace ecs {

template <typename Component>
class sparse_array {
    public:
        using value_type = std::optional<Component>;        // Each element is optional
        using reference_type = value_type&;                 // Reference to an element
        using const_reference_type = const value_type&;     // Const reference to an element
        using container_t = std::vector<value_type>;        // Vector to hold optional components
        using size_type = typename container_t::size_type;  // Type for sizes and indices

        sparse_array() = default;
        sparse_array(const sparse_array&) = default;
        sparse_array(sparse_array&&) noexcept = default;
        ~sparse_array() = default;

        sparse_array& operator=(const sparse_array&) = default;
        sparse_array& operator=(sparse_array&&) noexcept = default;

        reference_type operator[](size_type idx) { return _data[idx]; }
        const_reference_type operator[](size_type idx) const { return _data[idx]; }

        size_type size() const { return _data.size(); }

        reference_type insert_at(size_type pos, const Component& component) {
            if (pos >= _data.size()) {
                _data.resize(pos + 1);
            }
            _data[pos] = component;
            return _data[pos];
        }

        reference_type insert_at(size_type pos, Component&& component) {
            if (pos >= _data.size()) {
                _data.resize(pos + 1);
            }
            _data[pos] = std::forward<Component>(component);
            return _data[pos];
        }

        template <class... Params>
        reference_type emplace_at(size_type pos, Params&&... params) {
            if (pos >= _data.size()) {
                _data.resize(pos + 1);
            }
            _data[pos].emplace(std::forward<Params>(params)...);
            return _data[pos];
        }

        void erase(size_type pos) {
            if (pos < _data.size()) {
                _data[pos].reset();
            }
        }

        size_type get_index(const value_type& component) const {
            auto it = std::find(_data.begin(), _data.end(), component);
            return (it != _data.end()) ? std::distance(_data.begin(), it) : -1;
        }

        auto begin() { return _data.begin(); }
        auto end() { return _data.end(); }

        auto begin() const { return _data.begin(); }
        auto end() const { return _data.end(); }

    private:
        container_t _data;
};

}
