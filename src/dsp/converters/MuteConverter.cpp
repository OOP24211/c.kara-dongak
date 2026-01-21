#include "MuteConverter.h"

#include <cmath>

MuteConverter::MuteConverter(std::unique_ptr<ISampleSource> input, double startSec, double endSec)
    : in_(std::move(input)), startSec_(startSec), endSec_(endSec) {}

WavFormat MuteConverter::format() const {
    return in_->format();
}

bool MuteConverter::next(int16_t& sample) {
    int16_t inSample = 0;
    if (!in_->next(inSample)) {
        return false;
    }

    const auto fmt = in_->format();
    const double t = static_cast<double>(idx_) / static_cast<double>(fmt.sampleRate);

    if (t >= startSec_ && t < endSec_) {
        sample = 0;
    } else {
        sample = inSample;
    }

    ++idx_;
    return true;
}
