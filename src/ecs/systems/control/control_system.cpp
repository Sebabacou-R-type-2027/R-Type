/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_system
*/

#include "control_system.hpp"

namespace ecs::systems {

void ControlSystem::update(Registry& registry, client::Client& network) {
    auto& velocities = registry.get_components<Velocity>();
    auto& controllables = registry.get_components<Controllable>();

    for (std::size_t i = 0; i < controllables.size(); ++i) {
        if (controllables[i] && velocities[i]) {
            if (i == network.my_id_in_lobby_) {
	            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
	                network.send_message("CMDP|0");
    	        }
        	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
	                network.send_message("CMDP|1");
    	        }
        	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
	                network.send_message("CMDP|2");
    	        }
        	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
	                network.send_message("CMDP|3");
    	        }
            }
            for (auto it = network._commandsToDo.begin(); it != network._commandsToDo.end(); ) {
//                  std::cout << "command to do: " << it->first << " " << it->second << " my id: " << network.my_id_in_lobby_ << std::endl;
                  if (it->second == "0") {
                      velocities[std::stoi(it->first)]->vx += acceleration;
                  } else if (it->second == "1") {
                      velocities[std::stoi(it->first)]->vx -= acceleration;
                  } else if (it->second == "2") {
                      velocities[std::stoi(it->first)]->vy -= acceleration;
                  } else if (it->second == "3") {
                      velocities[std::stoi(it->first)]->vy += acceleration;
                  }
                  it = network._commandsToDo.erase(it);
			}
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                velocities[i]->vx *= 0.9f;
            }
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                velocities[i]->vy *= 0.9f;
            }

            velocities[i]->vx = std::clamp(velocities[i]->vx, -maxSpeed, maxSpeed);
            velocities[i]->vy = std::clamp(velocities[i]->vy, -maxSpeed, maxSpeed);
        }
    }
}

}