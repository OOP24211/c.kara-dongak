#pragma once

#include <memory>
#include <string>

#include "../dsp/ISampleSource.h"
#include "WavReader.h"

class WavSource : public ISampleSource {
public:
    explicit WavSource(const std::string& path)
        : reader_(path), fmt_(reader_.format()) {}

    WavFormat format() const override { return fmt_; }

    bool next(int16_t& sample) override {
        return reader_.readSample(sample);
    }

private:
    WavReader reader_;
    WavFormat fmt_;
};
