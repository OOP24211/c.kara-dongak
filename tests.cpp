#include <gtest/gtest.h>

#include "icache.h"
#include "lru_cache.h"
#include "lfu_cache.h"


// если get вернул пусто — считаем, что -1
int getOrMinusOne(ICache& cache, int key) {
    auto v = cache.get(key);
    return v.has_value() ? v.value() : -1;
}

// LRU

TEST(CacheTest, LRU_simple) {
    LRUCache cache(3);

    cache.put(1, 10); // freq(1)=1
    cache.put(2, 20); // freq(2)=1
    cache.put(3, 30); // freq(3)=2

    EXPECT_EQ(getOrMinusOne(cache, 1), 10); // освежили 1

    cache.put(4, 40); // кто-то должен вылететь

    EXPECT_EQ(getOrMinusOne(cache, 2), -1); // вылетает 2
    EXPECT_EQ(getOrMinusOne(cache, 1), 10);
    EXPECT_EQ(getOrMinusOne(cache, 3), 30);
    EXPECT_EQ(getOrMinusOne(cache, 4), 40);
}

TEST(CacheTest, LRU_hard) {
    LRUCache cache(1);

    cache.put(1, 10); // freq(1)=1
    EXPECT_EQ(getOrMinusOne(cache, 1), 10); // освежили 1

    cache.put(2, 20);
    EXPECT_EQ(getOrMinusOne(cache, 1), -1); // вылетает 1
    EXPECT_EQ(getOrMinusOne(cache, 2), 20);
}

// LFU

TEST(CacheTest, LFU_simple) {
    LFUCache cache(3);

    cache.put(1, 10); // freq(1)=1
    cache.put(2, 20); // freq(2)=1
    cache.put(3, 30); // freq(3)=2

    EXPECT_EQ(getOrMinusOne(cache, 1), 10); // +freq(1)
    EXPECT_EQ(getOrMinusOne(cache, 3), 30); // +freq(3)

    // freq(1) и freq(3) больше, чем у 2
    cache.put(4, 40);

    EXPECT_EQ(getOrMinusOne(cache, 2), -1); // вылетает 2
    EXPECT_EQ(getOrMinusOne(cache, 1), 10);
    EXPECT_EQ(getOrMinusOne(cache, 3), 30);
    EXPECT_EQ(getOrMinusOne(cache, 4), 40);
}

TEST(CacheTest, LFU_hard) {
    LFUCache cache(2);

    cache.put(1, 10); // freq(1)=1
    cache.put(2, 20); // freq(2)=1

    EXPECT_EQ(getOrMinusOne(cache, 1), 10); // freq(1)=2

    // freq(1)=2, freq(2)=1 → вылетает 2
    cache.put(3, 30);

    EXPECT_EQ(getOrMinusOne(cache, 2), -1); // вылетает 2
    EXPECT_EQ(getOrMinusOne(cache, 1), 10);
    EXPECT_EQ(getOrMinusOne(cache, 3), 30);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
