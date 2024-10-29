module;

#if __cpp_lib_modules < 202207L
#include <functional>
#endif
#include <SFML/Graphics.hpp>
export module game:components.buttons;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

using namespace ecs::components;

export namespace game::components {
    struct button : public gui::drawable {
        std::function<void()> callback;
        bool is_pressed = false;
        sf::Color default_color = sf::Color::Blue;
        sf::Color hover_color = sf::Color::Cyan;
        sf::Color click_color = sf::Color::Green;

        button(float width, float height, std::string_view label, const sf::Font& font, std::function<void()> action, ecs::entity display,
            const sf::Color &text_color = sf::Color::White, std::size_t text_size = 24) noexcept
            : gui::drawable{display, {}}, callback(std::move(action))
        {
            auto rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));
            rect->setOrigin(rect->getLocalBounds().width / 2, rect->getLocalBounds().height / 2);
            _rect = rect.get();
            auto rect_element = std::make_unique<gui::display_element>(std::move(rect));
            rect_element->reposition = gui::display_element::reposition_center;
            this->elements.insert({display, std::move(rect_element)});

            auto text = std::make_unique<sf::Text>(std::string(label), font, text_size);
            text->setOrigin(text->getLocalBounds().width / 2, text->getLocalBounds().height / 2);
            text->setFillColor(text_color);
            _text = text.get();
            auto text_element = std::make_unique<gui::display_element>(std::move(text), "arial");
            text_element->reposition = [&rect = *this->_rect](auto &text, const auto &position){
                static_cast<sf::Text &>(*text.element).setPosition(
                    position.x + rect.getLocalBounds().width / 2,
                    position.y + rect.getLocalBounds().height / 3
                );
            };
            this->elements.insert({display, std::move(text_element)});
        }

        inline void update(sf::Vector2f point) noexcept
        {
            if (_rect->getGlobalBounds().contains(point)) {
                _rect->setFillColor(hover_color);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    setScale(click_scale);
                    _rect->setFillColor(click_color);
                    if (!is_pressed)
                        callback();
                    is_pressed = true;
                } else {
                    setScale(hover_scale);
                    _rect->setFillColor(hover_color);
                    is_pressed = false;
                }
            } else {
                setScale(default_scale);
                _rect->setFillColor(default_color);
                is_pressed = false;
            }
        }

        private:
            static constexpr float default_scale = 1.0f, hover_scale = 1.05f, click_scale = 0.95f;
            sf::RectangleShape *_rect;
            sf::Text *_text;

            inline void setScale(float scale) noexcept
            {
                _rect->setScale(scale, scale);
                _text->setScale(scale, scale);
            }
    };
}
