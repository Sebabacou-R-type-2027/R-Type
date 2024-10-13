/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** LifeState
*/


#pragma once

namespace ecs {

    /**
     * @brief Class that contains the state of an entity
     */
    struct LifeState
    {
        bool isAlive; ///< State of the entity

        /**
         * @brief Construct a new Life State object
         * 
         * @param isAlive contains the state of the entity
         */
        LifeState(bool isAlive = true) : isAlive(isAlive) {};
    };
};
