#ifndef OOP_LAB1_ICACHE_H
#define OOP_LAB1_ICACHE_H
#pragma once
#include <optional>
#include <cstddef>

class ICache {
public:
    virtual ~ICache() = default;

    virtual std::optional<int> get(int key) = 0;
    virtual void put(int key, int value) = 0;

    virtual std::size_t size() = 0;
    virtual std::size_t capacity() = 0;
};

#endif //OOP_LAB1_ICACHE_H