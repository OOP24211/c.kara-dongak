#include "ConverterRegistry.h"

#include "../core/Exceptions.h"

void ConverterRegistry::add(ConverterInfo info) {
    if (info.name.empty()) {
        throw ConfigError("Конвертер без имени — так нельзя");
    }
    if (!info.build) {
        throw ConfigError("У конвертера '" + info.name + "' нет build-функции");
    }
    map_[info.name] = std::move(info);
}

bool ConverterRegistry::has(const std::string& name) const {
    return map_.find(name) != map_.end();
}

const ConverterInfo& ConverterRegistry::get(const std::string& name) const {
    auto it = map_.find(name);
    if (it == map_.end()) {
        throw ConfigError("Неизвестный конвертер: " + name);
    }
    return it->second;
}

std::vector<ConverterInfo> ConverterRegistry::all() const {
    std::vector<ConverterInfo> out;
    out.reserve(map_.size());
    for (const auto& kv : map_) {
        out.push_back(kv.second);
    }
    return out;
}
