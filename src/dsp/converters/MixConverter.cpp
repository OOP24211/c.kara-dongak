#include "MixConverter.h"

#include <cmath>

static int16_t clipToInt16(int v) {
    if (v > 32767) return 32767;
    if (v < -32768) return -32768;
    return static_cast<int16_t>(v);
}

MixConverter::MixConverter(std::unique_ptr<ISampleSource> main,
                           std::unique_ptr<ISampleSource> extra,
                           double startSec)
    : main_(std::move(main)), extra_(std::move(extra)) {
    const auto fmt = main_->format();
    startSample_ = static_cast<std::int64_t>(std::llround(startSec * static_cast<double>(fmt.sampleRate)));
    if (startSample_ < 0) startSample_ = 0;
}

WavFormat MixConverter::format() const {
    return main_->format();
}

bool MixConverter::next(int16_t& sample) {
    int16_t a = 0;
    if (!main_->next(a)) {
        return false;
    }

    if (idx_ < startSample_) {
        sample = a;
        ++idx_;
        return true;
    }

    int16_t b = 0;
    if (!extra_ || !extra_->next(b)) {
        sample = a;
        ++idx_;
        return true;
    }

    int out = (static_cast<int>(a) + static_cast<int>(b)) / 2;
    sample = clipToInt16(out);

    ++idx_;
    return true;
}
