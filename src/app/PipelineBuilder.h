#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../config/Command.h"
#include "../core/Context.h"
#include "../dsp/ISampleSource.h"
#include "ConverterRegistry.h"

class PipelineBuilder {
public:
    PipelineBuilder(const ConverterRegistry& registry, const Context& ctx);

    std::unique_ptr<ISampleSource> build(std::unique_ptr<ISampleSource> input,
                                         const std::vector<Command>& commands) const;

private:
    const ConverterRegistry& registry_;
    const Context& ctx_;
};
