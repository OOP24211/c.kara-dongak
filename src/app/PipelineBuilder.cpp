#include "PipelineBuilder.h"

PipelineBuilder::PipelineBuilder(const ConverterRegistry& registry, const Context& ctx)
    : registry_(registry), ctx_(ctx) {}

std::unique_ptr<ISampleSource> PipelineBuilder::build(
    std::unique_ptr<ISampleSource> input,
    const std::vector<Command>& commands
) const {
    std::unique_ptr<ISampleSource> current = std::move(input);

    for (const auto& cmd : commands) {
        const auto& info = registry_.get(cmd.name);
        current = info.build(std::move(current), cmd, ctx_);
    }

    return current;
}
