#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../config/Command.h"
#include "../core/Context.h"
#include "../dsp/ISampleSource.h"


struct ConverterInfo {
    std::string name;        // "gain"
    std::string syntax;      // "gain <factor>"
    std::string description; // "Умножает амплитуду на коэффициент..."

    // Фабрика: берёт входной поток + команду + контекст, возвращает новый поток
    std::function<std::unique_ptr<ISampleSource>(
        std::unique_ptr<ISampleSource> input,
        const Command& cmd,
        const Context& ctx
    )> build;
};

class ConverterRegistry {
public:
    void add(ConverterInfo info);

    bool has(const std::string& name) const;
    const ConverterInfo& get(const std::string& name) const;

    std::vector<ConverterInfo> all() const;

private:
    std::unordered_map<std::string, ConverterInfo> map_;
};
