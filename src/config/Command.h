#pragma once

#include <string>
#include <vector>

struct Command {
    std::string name;              // например: "gain"
    std::vector<std::string> args; // например: {"0.5"} или {"$2", "10"}
};

