#pragma once

#include <cstdint>
#include <memory>

#include "../ISampleSource.h"

class MuteConverter : public ISampleSource {
public:
    MuteConverter(std::unique_ptr<ISampleSource> input, double startSec, double endSec);

    WavFormat format() const override;
    bool next(int16_t& sample) override;

private:
    std::unique_ptr<ISampleSource> in_;
    double startSec_ = 0.0;
    double endSec_ = 0.0;

    std::int64_t idx_ = 0;
};
