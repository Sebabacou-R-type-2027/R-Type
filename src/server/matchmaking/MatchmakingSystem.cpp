//
// Created by shooting_star_t33 on 10/17/24.
//

#include "MatchmakingSystem.hpp"
#include <random>

void MatchmakingSystem::addPlayerToQueue(const server::client& cli) {
    this->queue_.add_player(cli);
}

void MatchmakingSystem::removePlayerFromQueue(const server::client& cli) {
    this->queue_.remove_player(cli);
}

void MatchmakingSystem::matchPlayers() {
    if (queue_.is_enough_player()) {
        std::queue<server::client> players = queue_.get_players();
        static std::random_device rd; // Seed for the random number engine
        static std::mt19937 gen(rd()); // Mersenne Twister engine
        static std::uniform_int_distribution<> dis(1, 1000000); // Uniform distribution

        int lobby_id = dis(gen); // Generate a random lobby ID
        Lobby lobby = Lobby(lobby_id, players.front());
        lobby.add_client(players.front());
        players.pop();
        for (int i = 0; i < 3; i++) {
            if (players.empty()) {
                break;
            }
            lobby.add_client(players.front());
            players.pop();
        }
        lobbies_.push_back(lobby);
    }
    std::cout << "quitting matchmaking" << std::endl;
}

void MatchmakingSystem::displayeQueue() const {
    std::queue<server::client> players = queue_.get_players();
    while (!players.empty()) {
        std::cout << players.front().get_id() << std::endl;
        players.pop();
    }
}

Lobby MatchmakingSystem::getLobby() const {
    if (lobbies_.empty()) {
          throw std::runtime_error("No lobby available");
    }
    return lobbies_.back();
}