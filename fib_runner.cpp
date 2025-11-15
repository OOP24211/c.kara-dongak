#include "fib_runner.h"
#include "fib.h"

FibRunner::FibRunner(ICache &cache) : cache_(cache) {}

int FibRunner::computeFib(int n) {
    return fibWithCache(n, cache_);
}


