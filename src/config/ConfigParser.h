#pragma once

#include <string>
#include <vector>

#include "Command.h"

class ConfigParser {
public:
    std::vector<Command> parseFile(const std::string& path) const;
};
