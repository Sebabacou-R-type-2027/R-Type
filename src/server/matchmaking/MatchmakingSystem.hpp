//
// Created by shooting_star_t33 on 10/17/24.
//

#ifndef MATCHMAKINGSYSTEM_HPP
#define MATCHMAKINGSYSTEM_HPP

#include <queue>
#include "client.hpp"
#include "MatchmakingQueue.hpp"
#include "lobby/Lobby.hpp"

class MatchmakingSystem {
    public:
        MatchmakingSystem() = default;
        ~MatchmakingSystem() = default;
        void addPlayerToQueue(server::client& cli);
        void removePlayerFromQueue(const server::client& cli);
        void matchPlayers();
        Lobby& getLobby();
        void displayeQueue() const;
    private:
         MatchmakingQueue queue_;
         std::vector<Lobby> lobbies_;
         int lobbyCounter = 0;
};



#endif //MATCHMAKINGSYSTEM_HPP
