#ifndef MATCHMAKINGQUEUE_HPP
#define MATCHMAKINGQUEUE_HPP

#include <memory>
#include <queue>
#include "client.hpp"

class MatchmakingQueue {
public:
    MatchmakingQueue() = default;
    ~MatchmakingQueue() = default;
    void add_player(const std::shared_ptr<server::client>& cli);
    void remove_player(const std::shared_ptr<server::client>& cli);
    [[nodiscard]] std::queue<std::shared_ptr<server::client>> get_players() const;
    [[nodiscard]] bool is_enough_player() const;
    void clear_queue();
private:
    std::queue<std::shared_ptr<server::client>> clients_queue_;
};

#endif // MATCHMAKINGQUEUE_HPP