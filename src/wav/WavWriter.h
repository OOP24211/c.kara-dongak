#pragma once

#include <cstdint>
#include <fstream>
#include <string>

#include "../dsp/ISampleSource.h"

class WavWriter {
public:
    WavWriter(const std::string& path, const WavFormat& fmt);
    ~WavWriter();

    void writeSample(int16_t sample);

private:
    std::ofstream out_;
    WavFormat fmt_{};

    std::uint32_t dataBytesWritten_ = 0;

private:
    static void writeU16LE(std::ostream& s, std::uint16_t v);
    static void writeU32LE(std::ostream& s, std::uint32_t v);

    void writeHeaderPlaceholder();
    void finalizeHeader();
};
