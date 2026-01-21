#pragma once

#include <string>
#include <vector>

namespace strutil {

    std::string trim(const std::string& s);

    std::vector<std::string> splitWs(const std::string& s);

    bool startsWith(const std::string& s, const std::string& prefix);

}
