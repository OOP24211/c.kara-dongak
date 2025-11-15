#include "fib.h"
#include "icache.h"

int fibWithCache(int n, ICache& cache) {
    if (n == 0) {
        return 0;
    }

    if (n == 1) {
        return 1;
    }

    auto val = cache.get(n);
    if (val.has_value()) {
        return val.value();
    }

    auto left = fibWithCache(n - 1, cache);
    auto right = fibWithCache(n - 2, cache);

    int result = left + right;

    cache.put(n, result);
    return result;
}