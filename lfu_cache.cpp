#include "lfu_cache.h"

LFUCache::LFUCache(std::size_t cap) : cap_(cap), size_(0), minFreq_(0) {}

void LFUCache::touch(int key) {
    auto itItem = items_.find(key);
    if (itItem == items_.end()) {
        return;
    }

    Entry &entry = itItem->second;
    int oldFreq = entry.freq;

    auto listIt = entry.it;
    auto &oldList = freqLists_[oldFreq];
    oldList.erase(listIt);

    if (oldList.empty()) {
        freqLists_.erase(oldFreq);
        if (minFreq_ == oldFreq) {
            ++minFreq_;
        }
    }
    int newFreq = oldFreq + 1;
    entry.freq = newFreq;

    auto &newList = freqLists_[newFreq];
    newList.push_back(key);
    entry.it = std::prev(newList.end());
}
void LFUCache::evictIfNeeded() {
    if (size_ < cap_) {
        return;
    }
    if (cap_ == 0) {
        return;
    }

    auto itList = freqLists_.find(minFreq_);
    if (itList == freqLists_.end()) {
        return;
    }

    auto &lst = itList->second;
    if (lst.empty()) {
        freqLists_.erase(itList);
    }

    int keyToRemove = lst.front();
    lst.pop_front();

    if (lst.empty()) {
        freqLists_.erase(itList);
    }

    items_.erase(keyToRemove);
    --size_;
}
std::optional<int> LFUCache::get(int key) {
    if (cap_ == 0) {
        return std::nullopt;
    }

    auto it = items_.find(key);
    if (it == items_.end()) {
        return std::nullopt;
    }

    int value = it->second.value;
    touch(key);

    return value;
}

void LFUCache::put(int key, int value) {
    if (cap_ == 0) {
        return;
    }

    auto it = items_.find(key);
    if (it != items_.end()) {
        it->second.value = value;
        touch(key);
        return;
    }

    if (cap_ == 0) {
        return;
    }

    if (size_ == cap_) {
        evictIfNeeded();
    }

    Entry entry;
    entry.value = value;
    entry.freq = 1;

    auto &listFreq1 = freqLists_[1];
    listFreq1.push_back(key);
    entry.it = std::prev(listFreq1.end());

    items_[key] = entry;
    ++size_;
    minFreq_ = 1;
}

std::size_t LFUCache::size() {
    return size_;
}

std::size_t LFUCache::capacity() {
    return cap_;
}