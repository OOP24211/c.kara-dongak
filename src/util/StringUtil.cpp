#include "StringUtil.h"

#include <cctype>

namespace strutil {

    std::string trim(const std::string& s) {
        std::size_t l = 0;
        while (l < s.size() && std::isspace(static_cast<unsigned char>(s[l]))) {
            ++l;
        }

        std::size_t r = s.size();
        while (r > l && std::isspace(static_cast<unsigned char>(s[r - 1]))) {
            --r;
        }

        return s.substr(l, r - l);
    }

    std::vector<std::string> splitWs(const std::string& s) {
        std::vector<std::string> out;

        std::size_t i = 0;
        while (i < s.size()) {
            while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) {
                ++i;
            }
            if (i >= s.size()) break;

            std::size_t j = i;
            while (j < s.size() && !std::isspace(static_cast<unsigned char>(s[j]))) {
                ++j;
            }
            out.emplace_back(s.substr(i, j - i));
            i = j;
        }

        return out;
    }

    bool startsWith(const std::string& s, const std::string& prefix) {
        if (prefix.size() > s.size()) return false;
        for (std::size_t i = 0; i < prefix.size(); ++i) {
            if (s[i] != prefix[i]) return false;
        }
        return true;
    }

}
