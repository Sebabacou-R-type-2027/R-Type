//
// Created by shooting_star_t33 on 10/17/24.
//

#ifndef MATCHMAKINGQUEUE_HPP
#define MATCHMAKINGQUEUE_HPP

#include <queue>
#include "client.hpp"

class MatchmakingQueue {
    public:
        MatchmakingQueue() = default;
        ~MatchmakingQueue() = default;
        void add_player(const server::client& cli);
        void remove_player(const server::client& cli);
        [[nodiscard]] std::queue<server::client> get_players() const;
        [[nodiscard]] bool is_enough_player() const;
        void clear_queue();
    private:
        std::queue<server::client> clients_queue_;
};



#endif //MATCHMAKINGQUEUE_HPP
