#pragma once

#include <cstdint>
#include <memory>

#include "../ISampleSource.h"

class MixConverter : public ISampleSource {
public:
    MixConverter(std::unique_ptr<ISampleSource> main,
                 std::unique_ptr<ISampleSource> extra,
                 double startSec);

    WavFormat format() const override;
    bool next(int16_t& sample) override;

private:
    std::unique_ptr<ISampleSource> main_;
    std::unique_ptr<ISampleSource> extra_;

    std::int64_t idx_ = 0;
    std::int64_t startSample_ = 0;
};
