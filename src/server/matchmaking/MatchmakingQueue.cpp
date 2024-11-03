#include "MatchmakingQueue.hpp"

void MatchmakingQueue::add_player(const std::shared_ptr<server::client>& cli) {
    this->clients_queue_.push(cli);
}

void MatchmakingQueue::remove_player(const std::shared_ptr<server::client>& cli) {
    std::queue<std::shared_ptr<server::client>> temp_queue;
    while (!clients_queue_.empty()) {
        if (clients_queue_.front()->get_id() != cli->get_id()) {
            temp_queue.push(clients_queue_.front());
        }
        clients_queue_.pop();
    }
    clients_queue_ = temp_queue;
}

std::queue<std::shared_ptr<server::client>> MatchmakingQueue::get_players() const {
    return clients_queue_;
}

bool MatchmakingQueue::is_enough_player() const {
    return clients_queue_.size() >= 4;
}

void MatchmakingQueue::clear_queue() {
    while (!clients_queue_.empty()) {
        clients_queue_.pop();
    }
}