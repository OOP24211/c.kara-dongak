#include "RegisterDefaultConverters.h"

#include <cstdlib>

#include "../core/Exceptions.h"
#include "../dsp/converters/GainConverter.h"
#include "../dsp/converters/MuteConverter.h"
#include "../dsp/converters/MixConverter.h"

static double parseDoubleStrict(const std::string& s, const std::string& prefix) {
    char* end = nullptr;
    double v = std::strtod(s.c_str(), &end);
    if (end == s.c_str() || *end != '\0') {
        throw ConfigError(prefix + "это не число: " + s);
    }
    return v;
}

static std::size_t parseInputRef(const std::string& s, const std::string& prefix) {
    if (s.size() < 2 || s[0] != '$') {
        throw ConfigError(prefix + "нужно указать вход как $N, а не " + s);
    }
    char* end = nullptr;
    long v = std::strtol(s.c_str() + 1, &end, 10);
    if (end == (s.c_str() + 1) || *end != '\0' || v <= 0) {
        throw ConfigError(prefix + "неверный номер входа: " + s);
    }
    return static_cast<std::size_t>(v);
}

void registerDefaultConverters(ConverterRegistry& registry) {

    // gain
    registry.add(ConverterInfo{
        "gain",
        "gain <factor>",
        "Делает звук тише или громче.",
        [](std::unique_ptr<ISampleSource> input, const Command& cmd, const Context&) {
            if (cmd.args.size() != 1) {
                throw ConfigError("gain: нужен один параметр (коэффициент)");
            }
            double factor = parseDoubleStrict(cmd.args[0], "gain: ");
            return std::make_unique<GainConverter>(std::move(input), factor);
        }
    });

    // mute
    registry.add(ConverterInfo{
        "mute",
        "mute <start_sec> <end_sec>",
        "Выключает звук на заданном промежутке.",
        [](std::unique_ptr<ISampleSource> input, const Command& cmd, const Context&) {
            if (cmd.args.size() != 2) {
                throw ConfigError("mute: нужно два числа — начало и конец");
            }
            double startSec = parseDoubleStrict(cmd.args[0], "mute: ");
            double endSec   = parseDoubleStrict(cmd.args[1], "mute: ");
            return std::make_unique<MuteConverter>(std::move(input), startSec, endSec);
        }
    });

    // mix
    registry.add(ConverterInfo{
        "mix",
        "mix <$N> <start_sec>",
        "Добавляет второй звук поверх основного.",
        [](std::unique_ptr<ISampleSource> input, const Command& cmd, const Context& ctx) {
            if (cmd.args.size() != 2) {
                throw ConfigError("mix: нужно указать вход и время начала");
            }

            std::size_t n = parseInputRef(cmd.args[0], "mix: ");
            double startSec = parseDoubleStrict(cmd.args[1], "mix: ");

            auto extra = ctx.openInput(n);

            if (input->format().sampleRate != extra->format().sampleRate) {
                throw WavFormatError("mix: разные sampleRate у файлов");
            }

            return std::make_unique<MixConverter>(
                std::move(input),
                std::move(extra),
                startSec
            );
        }
    });
}

