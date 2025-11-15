#ifndef OOP_LAB1_FIB_RUNNER_H
#define OOP_LAB1_FIB_RUNNER_H

#pragma once
#include "icache.h"

class FibRunner {
private:
    ICache& cache_;
public:
    explicit FibRunner(ICache& cache);

    int computeFib(int n);
};


#endif //OOP_LAB1_FIB_RUNNER_H