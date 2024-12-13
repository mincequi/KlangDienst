#include "Biquad.h"

#include <cmath>

Biquad::Biquad() {
    _history.resize(1, std::vector<History>(static_cast<size_t>(_channelCount)+1));
}

void Biquad::setCascadeCount(std::uint8_t count) {
    _history.resize(count, std::vector<History>(static_cast<size_t>(_channelCount)+1));
}

void Biquad::setSampleRate(std::uint32_t rate) {
    if (_sampleRate == rate) {
        return;
    }
    _sampleRate = rate;
    updateCoeffs();
}

FilterParams Biquad::filterParams() const {
    return _filterParams;
}

void Biquad::setFilterParams(const FilterParams& filter) {
    if (_filterParams == filter) {
        return;
    }
    _filterParams = filter;
    updateCoeffs();
}

void Biquad::process(AudioChannel channel_, const std::span<const float>& in_, const std::span<float>& out) {
    const auto channel = static_cast<std::size_t>(channel_);
    // Iterate cascades
    for (std::size_t cascadeIndex = 0; cascadeIndex < _history.size(); ++cascadeIndex) {
        // After first cascade, we have to process result and not again the input data
        const std::span<const float>& in = (cascadeIndex == 0) ? in_ : out;
        auto& h = _history.at(cascadeIndex).at(channel);
        for (std::size_t j = 0; j < in.size(); ++j) {
            double acc = 0.0;
            acc += _coeffs.b0*in[j];
            acc += _coeffs.b1*h.x1;
            acc += _coeffs.b2*h.x2;
            acc -= _coeffs.a1*h.y1;
            acc -= _coeffs.a2*h.y2;

            h.y2 = h.y1;
            h.y1 = acc;
            h.x2 = h.x1;
            h.x1 = in[j];

            out[j] = acc;
        }
    }
}

bool Biquad::isValid() const {
    return (_sampleRate > 0 && _filterParams.isValid());
}

bool Biquad::updateCoeffs() {
    if (_sampleRate == 0 || !_filterParams.isValid()) {
        return false;
    }

    double b0 = 0.0;
    double b1 = 0.0;
    double b2 = 0.0;
    double a0 = 0.0;
    double a1 = 0.0;
    double a2 = 0.0;

    switch (_filterParams.type) {
    case FilterType::Peaking: {
        double A = pow(10, _filterParams.g()/40.0);
        double w0 = 2*M_PI*_filterParams.f()/_sampleRate;
        double alpha = sin(w0)*0.5/_filterParams.q();
        double alpha1 = alpha*A;
        double alpha2 = alpha/A;

        a0 = 1.0 + alpha2;
        b0 = ( 1.0 + alpha1 ) / a0;
        b1 = (-2.0 * cos(w0)) / a0;
        b2 = ( 1.0 - alpha1 ) / a0;
        a1 = b1;
        a2 = ( 1.0 - alpha2 ) / a0;
        break;
    }
    case FilterType::LowPass: {
        double w0 = 2*M_PI*_filterParams.f()/_sampleRate;
        double alpha = sin(w0)*0.5/_filterParams.q();
        double cosw0 = cos(w0);

        a0 = 1.0 + alpha;
        b1 = ( 1.0 - cosw0 ) / a0;
        b0 = b1 * 0.5;
        b2 = b0;
        a1 = (-2.0 * cosw0 ) / a0;
        a2 = ( 1.0 - alpha ) / a0;
        break;
    }
    case FilterType::HighPass: {
        double w0 = 2*M_PI*_filterParams.f()/_sampleRate;
        double alpha = sin(w0)*0.5/_filterParams.q();
        double cosw0 = cos(w0);

        a0    = 1.0 + alpha;
        b1 = -( 1.0 + cosw0 ) / a0;
        b0 = b1 * -0.5;
        b2 = b0;
        a1 = (-2.0 * cosw0) / a0;
        a2 = ( 1.0 - alpha  ) / a0;
        break;
    }
    case FilterType::LowShelf: {
        double A = pow(10, _filterParams.g()/40.0);
        double w0 = 2*M_PI*_filterParams.f()/_sampleRate;
        double cosW0 = cos(w0);
        double alpha = sin(w0)*0.5/_filterParams.q();
        double sqrtAalpha2 = 2.0*sqrt(A)*alpha;

        a0 = (A+1) + (A-1)*cosW0 + sqrtAalpha2;
        b0 =    A*( (A+1) - (A-1)*cosW0 + sqrtAalpha2) / a0;
        b1 =  2*A*( (A-1) - (A+1)*cosW0              ) / a0;
        b2 =    A*( (A+1) - (A-1)*cosW0 - sqrtAalpha2) / a0;
        a1 =   -2*( (A-1) + (A+1)*cosW0              ) / a0;
        a2 =      ( (A+1) + (A-1)*cosW0 - sqrtAalpha2) / a0;
        break;
    }
    case FilterType::HighShelf: {
        double A = pow(10, _filterParams.g()/40.0);
        double w0 = 2*M_PI*_filterParams.f()/_sampleRate;
        double cosW0 = cos(w0);
        double alpha = sin(w0)*0.5/_filterParams.q();
        double sqrtAalpha2 = 2.0*sqrt(A)*alpha;

        a0 = (A+1) - (A-1)*cosW0 + sqrtAalpha2;
        b0 =    A*( (A+1) + (A-1)*cosW0 + sqrtAalpha2) / a0;
        b1 = -2*A*( (A-1) + (A+1)*cosW0              ) / a0;
        b2 =    A*( (A+1) + (A-1)*cosW0 - sqrtAalpha2) / a0;
        a1 =    2*( (A-1) - (A+1)*cosW0              ) / a0;
        a2 =      ( (A+1) - (A-1)*cosW0 - sqrtAalpha2) / a0;
        break;
    }
    case FilterType::Bypass:
    case FilterType::AllPass:
    case FilterType::Loudness:
        return false;
    }

    _coeffs.b0 = b0;
    _coeffs.b1 = b1;
    _coeffs.b2 = b2;
    _coeffs.a1 = a1;
    _coeffs.a2 = a2;

    return true;
}
