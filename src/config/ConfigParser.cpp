#include "ConfigParser.h"

#include <fstream>
#include <sstream>

#include "../core/Exceptions.h"
#include "../util/StringUtil.h"

std::vector<Command> ConfigParser::parseFile(const std::string& path) const {
    std::ifstream in(path);
    if (!in) {
        throw IoError("Не удалось открыть конфиг: " + path);
    }

    std::vector<Command> commands;

    std::string line;
    std::size_t lineNo = 0;

    while (std::getline(in, line)) {
        ++lineNo;

        std::string t = strutil::trim(line);
        if (t.empty()) continue;
        if (strutil::startsWith(t, "#")) continue;

        auto tokens = strutil::splitWs(t);
        if (tokens.empty()) continue;

        Command cmd;
        cmd.name = tokens[0];
        for (std::size_t i = 1; i < tokens.size(); ++i) {
            cmd.args.push_back(tokens[i]);
        }

        commands.push_back(std::move(cmd));
    }

    return commands;
}
