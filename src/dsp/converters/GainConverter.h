#pragma once

#include <cstdint>
#include <memory>

#include "../ISampleSource.h"

class GainConverter : public ISampleSource {
public:
    GainConverter(std::unique_ptr<ISampleSource> input, double factor);

    WavFormat format() const override;
    bool next(int16_t& sample) override;

private:
    std::unique_ptr<ISampleSource> in_;
    double factor_ = 1.0;

private:
    static int16_t clipToInt16(int v);
};
