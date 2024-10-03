// /*
// ** EPITECH PROJECT, 2024
// ** R-Type
// ** File description:
// ** Menu
// */

// #include "menu.hpp"
// #include <SFML/Window/Mouse.hpp>
// #include <iostream>  // For error handling

// namespace rtype {

//     // Constructor
//     Menu::Menu(ecs::Registry& registry) : registry(registry) {
//         // Initialize any components or settings if necessary
//     }

//     // Render the menu
//     void Menu::render(sf::RenderWindow& window) {
//         // Iterate over all entities with MenuComponent
//         for (auto& entity : registry.getEntitiesWithComponent<ecs::MenuComponent>()) {
//             const auto& menuComp = registry.getComponent<ecs::MenuComponent>(entity);

//             // Load the font
//             sf::Font font;
//             if (!font.loadFromFile("assets/fonts/NimbusSanL-Bol.otf")) {
//                 std::cerr << "Error loading font!" << std::endl;
//                 return;
//             }

//             // Create and render the title
//             sf::Text titleText;
//             titleText.setFont(font);
//             titleText.setString("R-Type");
//             titleText.setCharacterSize(50);
//             titleText.setFillColor(sf::Color::White);
//             titleText.setPosition(window.getSize().x / 2 - titleText.getGlobalBounds().width / 2, 50);
//             window.draw(titleText);

//             // Draw each button
//             for (const auto& button : menuComp.buttons) {
//                 window.draw(button.shape); // Draw the button's shape
//                 window.draw(button.text);  // Draw the button's text
//             }
//         }
//     }

//     // Handle user input
//     void Menu::handleInput(const sf::Event& event, sf::RenderWindow& window) {
//         if (event.type == sf::Event::MouseButtonPressed) {
//             checkButtonClicks(window);
//         }
//     }

//     // Check if any buttons have been clicked
//     void Menu::checkButtonClicks(sf::RenderWindow& window) {
//         // Get the current mouse position relative to the window
//         auto mousePosition = sf::Mouse::getPosition(window);

//         // Iterate over all entities with MenuComponent
//         for (auto& entity : registry.getEntitiesWithComponent<ecs::MenuComponent>()) {
//             const auto& menuComp = registry.getComponent<ecs::MenuComponent>(entity);

//             // Iterate over each button and check for click events
//             for (const auto& button : menuComp.buttons) {
//                 if (button.shape.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
//                     // Trigger the button's onClick action if it exists
//                     if (button.onClick) {
//                         button.onClick();
//                     }
//                 }
//             }
//         }
//     }

// }
