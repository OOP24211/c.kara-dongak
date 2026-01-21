#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../dsp/ISampleSource.h"

struct Context {
    std::vector<std::string> inputPaths;

    std::function<std::unique_ptr<ISampleSource>(std::size_t oneBasedIndex)> openInput;
};
