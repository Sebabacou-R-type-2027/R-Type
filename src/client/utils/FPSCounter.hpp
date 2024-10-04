/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** FPSCounter
*/

#pragma once
#include <SFML/System/Clock.hpp>

class FPSCounter {
public:
    FPSCounter() : m_frameCount(0), m_fps(0) {}

    void update() {
        m_frameCount++;
        if (m_clock.getElapsedTime().asSeconds() >= 1.0f) {
            m_fps = m_frameCount;
            m_frameCount = 0;
            m_clock.restart();
        }
    }

    int getFPS() const {
        return m_fps;
    }

private:
    sf::Clock m_clock;
    int m_frameCount;
    int m_fps;
};
