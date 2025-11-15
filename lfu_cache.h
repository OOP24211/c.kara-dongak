#ifndef OOP_LAB1_LFU_CACHE_H
#define OOP_LAB1_LFU_CACHE_H

#pragma once

#include "icache.h"
#include <list>
#include <unordered_map>

class LFUCache : public ICache {
private:
    std::size_t cap_;
    std::size_t size_;

    struct Entry {
        int value{};
        int freq{};
        std::list<int>::iterator it;
    };

    std::unordered_map<int, Entry> items_;
    std::unordered_map<int, std::list<int>> freqLists_;

    int minFreq_;
    void touch(int key);
    void evictIfNeeded();
public:
    explicit LFUCache(std::size_t cap);

    std::optional<int> get(int key) override;
    void put(int key, int value) override;

    std::size_t size() override;
    std::size_t capacity() override;
};

#endif