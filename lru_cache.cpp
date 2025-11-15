#include "lru_cache.h"

LRUCache::LRUCache(std::size_t capacity) : cap_(capacity){}

std::optional<int> LRUCache::get(int key) {
    if (cap_ == 0) {
        return std::nullopt;
    }

    auto it = pos_.find(key);
    if (it == pos_.end()) {
        return std::nullopt;
    }

    auto listIt = it->second;

    int value = listIt->second;
    order_.splice(order_.begin(), order_, listIt);

    return value;
}

void LRUCache::put(int key, int value) {
    if (cap_ == 0) {
        return;
    }
    auto it = pos_.find(key);
    if (it != pos_.end()) {
        auto listIt = it->second;
        listIt->second = value;
        order_.splice(order_.begin(), order_, listIt);
        return;
    }
    if (order_.size() >= cap_) {
        auto &backPair = order_.back();
        int oldKey = backPair.first;

        pos_.erase(oldKey);
        order_.pop_back();
    }
    order_.emplace_front(key, value);
    pos_[key] = order_.begin();
}

std::size_t LRUCache::size() {
    return order_.size();
}
std::size_t LRUCache::capacity() {
    return cap_;
}


