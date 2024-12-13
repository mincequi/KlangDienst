#pragma once

#include <cstdint>
#include <span>
#include <vector>

#include <AudioChannel.h>
#include <FilterParams.h>

class Biquad {
public:
    enum class Type {
        Invalid     = 0,
        Peak        = 1,
        LowPass,
        HighPass,
        LowShelf,
        HighShelf,
        AllPass,
    };

    Biquad();

    /**
     * @brief Set number of cascades/passes.
     *
     * A biquad can have multiple cascades/passes to be applied to sample data.
     *
     * @param count
     */
    void setCascadeCount(uint8_t count);

    /**
     * @brief Set the sample rate.
     *
     * The sample rate is required to calculate the filter coefficients.
     *
     * @param rate
     */
    void setSampleRate(uint32_t rate);

    /**
     * @brief Get the filter parameters.
     *
     * @return FilterParams
     */
    FilterParams filterParams() const;

    /**
     * @brief Set the filter parameters.
     *
     * The filter parameters are used to calculate the filter coefficients.
     *
     * @param type
     */
    void setFilterParams(const FilterParams& type);

    /**
     * @brief Process a span of audio data.
     *
     * @param channel
     * @param in_
     * @param out
     */
    void process(AudioChannel channel, const std::span<const float>& in_, const std::span<float>& out);

    /**
     * @brief Check if the biquad is valid.
     *
     * @return true if valid, false otherwise
     */
    bool isValid() const;

private:
    bool updateCoeffs();

    // The properties of a biquad
    AudioChannel _channelCount = AudioChannel::FR;
    uint32_t _sampleRate = 0;
    FilterParams _filterParams;

    struct Coeffs {
        float b0 = 0.0, b1 = 0.0, b2 = 0.0;
        float a1 = 0.0, a2 = 0.0;
    };
    Coeffs _coeffs;

    // We have a history for each channel and cascade
    struct History {
        float x1 = 0.0, x2 = 0.0;
        float y1 = 0.0, y2 = 0.0;
    };
    std::vector<std::vector<History>> _history;
};
