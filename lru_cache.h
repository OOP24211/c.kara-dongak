#ifndef OOP_LAB1_LRU_CACHE_H
#define OOP_LAB1_LRU_CACHE_H

#pragma once

#include "icache.h"
#include <list>
#include <unordered_map>

class LRUCache : public ICache {
private:
    std::size_t cap_;
    std::list<std::pair<int, int>> order_;
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> pos_;
public:
    explicit LRUCache(std::size_t cap);

    std::optional<int> get(int key) override;
    void put(int key, int value) override;

    std::size_t size() override;
    std::size_t capacity() override;
};

#endif