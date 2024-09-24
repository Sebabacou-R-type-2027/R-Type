#include <SFML/Graphics.hpp>

class RoundedRectangle : public sf::Drawable {
public:
    RoundedRectangle(const sf::Vector2f& size, float radius)
        : m_radius(radius), m_size(size) {
        updateShape();
    }

    void setPosition(const sf::Vector2f& position) {
        m_shape.setPosition(position);
    }

    void setFillColor(const sf::Color& color) {
        m_shape.setFillColor(color);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_shape, states);
    }

private:
    float m_radius;
    sf::Vector2f m_size;
    sf::ConvexShape m_shape;

    void updateShape() {
        // Create a convex shape representing the rounded rectangle
        m_shape.setPointCount(8); // For simplicity, create a rough rounded rectangle
        // Set points based on size and radius
        m_shape.setPoint(0, sf::Vector2f(0, m_radius));
        m_shape.setPoint(1, sf::Vector2f(m_size.x - m_radius, 0));
        m_shape.setPoint(2, sf::Vector2f(m_size.x, m_radius));
        m_shape.setPoint(3, sf::Vector2f(m_size.x, m_size.y - m_radius));
        m_shape.setPoint(4, sf::Vector2f(m_size.x - m_radius, m_size.y));
        m_shape.setPoint(5, sf::Vector2f(m_radius, m_size.y));
        m_shape.setPoint(6, sf::Vector2f(0, m_size.y - m_radius));
        m_shape.setPoint(7, sf::Vector2f(0, m_radius));
    }
};
