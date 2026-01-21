#include "WavReader.h"

#include <cstring>
#include <sstream>

#include "../core/Exceptions.h"

static bool idEquals(const char id[4], const char* s4) {
    return std::memcmp(id, s4, 4) == 0;
}

WavReader::WavReader(const std::string& path) : in_(path, std::ios::binary) {
    if (!in_) {
        throw IoError("Не удалось открыть WAV файл: " + path);
    }
    parseHeader();
}

bool WavReader::readSample(int16_t& sample) {
    if (dataRead_ >= dataSize_) return false;

    char b[2];
    readExact(in_, b, 2);

    sample = static_cast<int16_t>(
        (static_cast<uint16_t>(static_cast<unsigned char>(b[0]))      ) |
        (static_cast<uint16_t>(static_cast<unsigned char>(b[1])) << 8 )
    );

    dataRead_ += 2;
    return true;
}

std::uint32_t WavReader::readU32LE(std::istream& s) {
    char b[4];
    readExact(s, b, 4);
    return (static_cast<std::uint32_t>(static_cast<unsigned char>(b[0]))      ) |
           (static_cast<std::uint32_t>(static_cast<unsigned char>(b[1])) << 8 ) |
           (static_cast<std::uint32_t>(static_cast<unsigned char>(b[2])) << 16) |
           (static_cast<std::uint32_t>(static_cast<unsigned char>(b[3])) << 24);
}

std::uint16_t WavReader::readU16LE(std::istream& s) {
    char b[2];
    readExact(s, b, 2);
    return (static_cast<std::uint16_t>(static_cast<unsigned char>(b[0]))      ) |
           (static_cast<std::uint16_t>(static_cast<unsigned char>(b[1])) << 8 );
}

void WavReader::readExact(std::istream& s, char* buf, std::size_t n) {
    s.read(buf, static_cast<std::streamsize>(n));
    if (s.gcount() != static_cast<std::streamsize>(n) || !s) {
        throw IoError("Ошибка чтения WAV (неожиданный конец файла)");
    }
}

void WavReader::parseHeader() {
    char riff[4];
    readExact(in_, riff, 4);
    if (!idEquals(riff, "RIFF")) {
        throw WavFormatError("Это не RIFF WAV (нет RIFF)");
    }

    (void)readU32LE(in_);

    char wave[4];
    readExact(in_, wave, 4);
    if (!idEquals(wave, "WAVE")) {
        throw WavFormatError("Это не WAVE (нет WAVE)");
    }

    bool gotFmt = false;
    bool gotData = false;

    std::uint16_t audioFormat = 0;
    std::uint16_t numChannels = 0;
    std::uint32_t sampleRate = 0;
    std::uint16_t bitsPerSample = 0;

    while (!gotData) {
        char chunkId[4];
        if (!in_.read(chunkId, 4)) {
            break;
        }
        std::uint32_t chunkSize = readU32LE(in_);

        if (idEquals(chunkId, "fmt ")) {
            if (chunkSize < 16) {
                throw WavFormatError("Некорректный fmt chunk (слишком маленький)");
            }

            audioFormat   = readU16LE(in_);
            numChannels   = readU16LE(in_);
            sampleRate    = readU32LE(in_);
            (void)readU32LE(in_); // byteRate
            (void)readU16LE(in_); // blockAlign
            bitsPerSample = readU16LE(in_);

            std::uint32_t remaining = chunkSize - 16;
            if (remaining > 0) {
                in_.seekg(static_cast<std::streamoff>(remaining), std::ios::cur);
                if (!in_) throw IoError("Ошибка при пропуске данных fmt chunk");
            }

            gotFmt = true;
        } else if (idEquals(chunkId, "data")) {
            dataSize_ = chunkSize;
            dataRead_ = 0;
            gotData = true;
        } else {
            in_.seekg(static_cast<std::streamoff>(chunkSize), std::ios::cur);
            if (!in_) throw IoError("Ошибка при пропуске неизвестного chunk WAV");
        }

        if (chunkSize % 2 == 1) {
            in_.seekg(1, std::ios::cur);
            if (!in_) throw IoError("Ошибка при пропуске pad byte WAV");
        }
    }

    if (!gotFmt) throw WavFormatError("В WAV не найден fmt chunk");
    if (!gotData) throw WavFormatError("В WAV не найден data chunk");

    if (audioFormat != 1) {
        throw WavFormatError("Поддерживается только PCM (audioFormat=1)");
    }
    if (numChannels != 1) {
        throw WavFormatError("Поддерживается только mono (1 канал)");
    }
    if (bitsPerSample != 16) {
        throw WavFormatError("Поддерживается только 16-bit PCM");
    }

    fmt_.channels = numChannels;
    fmt_.sampleRate = sampleRate;
    fmt_.bitsPerSample = bitsPerSample;

    if (dataSize_ % 2 != 0) {
        throw WavFormatError("Некорректный data chunk (размер не кратен 2)");
    }
}
