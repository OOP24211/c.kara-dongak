#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "app/ConverterRegistry.h"
#include "app/PipelineBuilder.h"
#include "app/RegisterDefaultConverters.h"
#include "config/ConfigParser.h"
#include "core/Context.h"
#include "core/Exceptions.h"
#include "wav/WavSource.h"
#include "wav/WavWriter.h"

static void printUsage(const ConverterRegistry& registry) {
    std::cout
        << "Usage:\n"
        << "  sound_processor -h\n"
        << "  sound_processor <config.txt> <output.wav> <input1.wav> [input2.wav ...]\n\n"
        << "Converters:\n";

    auto list = registry.all();
    for (const auto& c : list) {
        std::cout << "  " << c.syntax << "\n"
                  << "    " << c.description << "\n";
    }
}

int main(int argc, char** argv) {
    try {
        ConverterRegistry registry;
        registerDefaultConverters(registry);

        if (argc == 2 && std::string(argv[1]) == "-h") {
            printUsage(registry);
            return 0;
        }

        if (argc < 4) {
            printUsage(registry);
            throw CliError("Неправильно написали. Нужно: <config.txt> <output.wav> <input1.wav> [input2.wav ...]");
        }

        const std::string configPath = argv[1];
        const std::string outputPath = argv[2];

        std::vector<std::string> inputs;
        for (int i = 3; i < argc; ++i) {
            inputs.push_back(argv[i]);
        }

        Context ctx;
        ctx.inputPaths = inputs;
        ctx.openInput = [&ctx](std::size_t oneBasedIndex) -> std::unique_ptr<ISampleSource> {
            if (oneBasedIndex == 0 || oneBasedIndex > ctx.inputPaths.size()) {
                throw ConfigError("Неправильный индекс входного файла: $" + std::to_string(oneBasedIndex));
            }
            return std::make_unique<WavSource>(ctx.inputPaths[oneBasedIndex - 1]);
        };

        ConfigParser parser;
        auto commands = parser.parseFile(configPath);

        auto input1 = ctx.openInput(1);
        PipelineBuilder builder(registry, ctx);
        auto pipeline = builder.build(std::move(input1), commands);

        WavWriter writer(outputPath, pipeline->format());
        int16_t s = 0;
        while (pipeline->next(s)) {
            writer.writeSample(s);
        }

        return 0;
    }
    catch (const CliError& e) {
        std::cerr << "CLI error: " << e.what() << "\n";
        return 1;
    }
    catch (const IoError& e) {
        std::cerr << "IO error: " << e.what() << "\n";
        return 2;
    }
    catch (const WavFormatError& e) {
        std::cerr << "WAV format error: " << e.what() << "\n";
        return 3;
    }
    catch (const ConfigError& e) {
        std::cerr << "Config error: " << e.what() << "\n";
        return 4;
    }
    catch (const std::exception& e) {
        std::cerr << "Unknown error: " << e.what() << "\n";
        return 5;
    }
}
