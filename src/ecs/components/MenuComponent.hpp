#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

namespace ecs {

    struct MenuButton {
        sf::RectangleShape shape;
        sf::Text text;
        std::function<void()> onClick;

        // Optionally, add a method to update the button appearance on hover/click
        void updateAppearance(/* parameters for hover/click state */) {
            // Update the shape and text appearance based on state
        }
    };

    struct MenuComponent {
        std::string title = "R-Type";
        std::vector<MenuButton> buttons;

        MenuComponent() {
            // Initialize buttons
            buttons.push_back(createButton("Solo Campaign", []() { /* Action for Solo Campaign */ }));
            buttons.push_back(createButton("Multiplayer", []() { /* Action for Multiplayer */ }));
            buttons.push_back(createButton("Customize Shop", []() { /* Action for Customize Shop */ }));
            buttons.push_back(createButton("Map Editor", []() { /* Action for Map Editor */ }));
            buttons.push_back(createButton("Game Options", []() { /* Action for Game Options */ }));
            buttons.push_back(createButton("Quit", []() { /* Action for Quit */ }));
        }

    private:
        MenuButton createButton(const std::string& label, std::function<void()> action) {
            MenuButton button;

            // Set up button shape
            button.shape.setSize(sf::Vector2f(200, 50)); // Example size
            button.shape.setFillColor(sf::Color::Blue); // Example color

            // Set up button text
            button.text.setString(label);
            button.text.setCharacterSize(24); // Set character size
            button.text.setFillColor(sf::Color::White); // Set text color

            // Center the text within the button
            button.text.setPosition(
                button.shape.getPosition().x + (button.shape.getSize().x - button.text.getLocalBounds().width) / 2,
                button.shape.getPosition().y + (button.shape.getSize().y - button.text.getLocalBounds().height) / 2
            );

            button.onClick = action;
            return button;
        }
    };

}
