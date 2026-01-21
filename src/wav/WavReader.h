#pragma once

#include <cstdint>
#include <fstream>
#include <string>

#include "../dsp/ISampleSource.h"

class WavReader {
public:
    explicit WavReader(const std::string& path);

    WavFormat format() const { return fmt_; }

    bool readSample(int16_t& sample);

private:
    std::ifstream in_;
    WavFormat fmt_{};

    std::uint32_t dataSize_ = 0;
    std::uint32_t dataRead_ = 0;

private:
    static std::uint32_t readU32LE(std::istream& s);
    static std::uint16_t readU16LE(std::istream& s);
    static void readExact(std::istream& s, char* buf, std::size_t n);

    void parseHeader();
};
