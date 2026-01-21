#include "WavWriter.h"

#include "../core/Exceptions.h"

WavWriter::WavWriter(const std::string& path, const WavFormat& fmt)
    : out_(path, std::ios::binary), fmt_(fmt) {
    if (!out_) {
        throw IoError("Не удалось открыть output WAV для записи: " + path);
    }

    if (fmt_.channels != 1) {
        throw WavFormatError("WavWriter: поддерживается только mono (1 канал)");
    }
    if (fmt_.bitsPerSample != 16) {
        throw WavFormatError("WavWriter: поддерживается только 16-bit PCM");
    }

    writeHeaderPlaceholder();
}

WavWriter::~WavWriter() {
    try {
        finalizeHeader();
    } catch (...) {
    }
}

void WavWriter::writeSample(int16_t sample) {
    char b[2];
    b[0] = static_cast<char>(sample & 0xFF);
    b[1] = static_cast<char>((sample >> 8) & 0xFF);

    out_.write(b, 2);
    if (!out_) {
        throw IoError("Ошибка записи WAV (writeSample)");
    }
    dataBytesWritten_ += 2;
}

void WavWriter::writeU16LE(std::ostream& s, std::uint16_t v) {
    char b[2];
    b[0] = static_cast<char>(v & 0xFF);
    b[1] = static_cast<char>((v >> 8) & 0xFF);
    s.write(b, 2);
}

void WavWriter::writeU32LE(std::ostream& s, std::uint32_t v) {
    char b[4];
    b[0] = static_cast<char>(v & 0xFF);
    b[1] = static_cast<char>((v >> 8) & 0xFF);
    b[2] = static_cast<char>((v >> 16) & 0xFF);
    b[3] = static_cast<char>((v >> 24) & 0xFF);
    s.write(b, 4);
}

void WavWriter::writeHeaderPlaceholder() {
    out_.write("RIFF", 4);
    writeU32LE(out_, 0);
    out_.write("WAVE", 4);

    out_.write("fmt ", 4);
    writeU32LE(out_, 16);
    writeU16LE(out_, 1);
    writeU16LE(out_, fmt_.channels);
    writeU32LE(out_, fmt_.sampleRate);

    std::uint16_t blockAlign = static_cast<std::uint16_t>(fmt_.channels * (fmt_.bitsPerSample / 8));
    std::uint32_t byteRate   = fmt_.sampleRate * blockAlign;

    writeU32LE(out_, byteRate);
    writeU16LE(out_, blockAlign);
    writeU16LE(out_, fmt_.bitsPerSample);

    out_.write("data", 4);
    writeU32LE(out_, 0);

    if (!out_) {
        throw IoError("Ошибка записи WAV заголовка");
    }
}

void WavWriter::finalizeHeader() {
    if (!out_) return;

    std::uint32_t dataSize = dataBytesWritten_;

    std::uint32_t riffSize = 4 + (8 + 16) + (8 + dataSize);

    out_.seekp(4, std::ios::beg);
    if (!out_) return;
    writeU32LE(out_, riffSize);

    out_.seekp(40, std::ios::beg);
    if (!out_) return;
    writeU32LE(out_, dataSize);

    out_.flush();
}
