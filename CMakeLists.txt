cmake_minimum_required(VERSION 3.16)
project(sound_processor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(sound_processor
        src/main.cpp
        src/dsp/ISampleSource.h
        src/core/Exceptions.h
        src/wav/WavReader.h
        src/wav/WavReader.cpp
        src/wav/WavWriter.h
        src/wav/WavWriter.cpp
        src/wav/WavSource.h
        src/main.cpp
        src/dsp/converters/GainConverter.h
        src/dsp/converters/GainConverter.cpp
        src/config/Command.h
        src/util/StringUtil.h
        src/util/StringUtil.cpp
        src/config/ConfigParser.h
        src/config/ConfigParser.cpp
        src/core/Context.h
        src/app/ConverterRegistry.h
        src/app/ConverterRegistry.cpp
        src/app/PipelineBuilder.h
        src/app/PipelineBuilder.cpp
        src/app/RegisterDefaultConverters.h
        src/app/RegisterDefaultConverters.cpp
        src/dsp/converters/MuteConverter.h
        src/dsp/converters/MuteConverter.cpp
        src/dsp/converters/MixConverter.h
        src/dsp/converters/MixConverter.cpp)

target_include_directories(sound_processor PRIVATE src)
