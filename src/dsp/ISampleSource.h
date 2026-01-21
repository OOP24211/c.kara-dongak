#pragma once

#include <cstdint>

struct WavFormat {
    uint16_t channels = 1;
    uint32_t sampleRate = 44100;
    uint16_t bitsPerSample = 16;
};

class ISampleSource {
public:
    virtual ~ISampleSource() = default;

    virtual WavFormat format() const = 0;

    virtual bool next(int16_t& sample) = 0;
};
