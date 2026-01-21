#include "GainConverter.h"

#include <cmath>

GainConverter::GainConverter(std::unique_ptr<ISampleSource> input, double factor)
    : in_(std::move(input)), factor_(factor) {}

WavFormat GainConverter::format() const {
    return in_->format();
}

int16_t GainConverter::clipToInt16(int v) {
    if (v > 32767) return 32767;
    if (v < -32768) return -32768;
    return static_cast<int16_t>(v);
}

bool GainConverter::next(int16_t& sample) {
    int16_t inSample = 0;
    if (!in_->next(inSample)) {
        return false;
    }

    double scaled = static_cast<double>(inSample) * factor_;
    int out = static_cast<int>(std::lround(scaled));

    sample = clipToInt16(out);
    return true;
}
