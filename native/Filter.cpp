#define _USE_MATH_DEFINES
#include "Filter.h"
#include <math.h>
#include <iostream>

namespace fx_rack {
  Filter::Filter(
    const pAudioFilter& child,
    FilterType type,
    unsigned int num_taps, double cutoff
  ): AudioFilter(child), num_taps(num_taps), cutoff(cutoff), type(type) {
    coeffs = new double[num_taps];
    buffer = new audio_sample_t[num_taps];

    for (size_t i = 0; i < num_taps; i++) buffer[i] = 0;
    type == LPF ? computeLPF() : computeHPF();
    normalizeCoeffs();
  }

  Filter::~Filter() {
    delete[] coeffs;
    delete[] buffer;
  }

  void Filter::normalizeCoeffs() {
    double sum = 0.0;
    for (size_t i = 0; i < num_taps; i++) sum += abs(coeffs[i]);
    for (size_t i = 0; i < num_taps; i++) coeffs[i] = coeffs[i] / sum;
  }

  void Filter::computeLPF() {
    double omega = 2.0 * M_PI * (cutoff / sampFreq);

    for (size_t i = 0; i < num_taps; i++) {
      size_t n = i - (num_taps / 2);
      if (n != 0) {
        coeffs[i] = sin(omega * n) / (M_PI * n);
      } else {
        coeffs[i] = omega / M_PI;
      }
    }
  }

  void Filter::computeHPF() {
    computeLPF();
    for (size_t i = 0; i < num_taps; i++) coeffs[i] = -coeffs[i];
    coeffs[(num_taps - 1) / 2] += 1.0;
  }

  void Filter::process_sample(audio_sample_t& sample) {
    for (size_t i = num_taps - 1; i > 0; i--) {
      buffer[i] = buffer[i - 1];
    }
    buffer[0] = sample;

    double left = 0.0;
    double right = 0.0;

    for (size_t i = 0; i < num_taps; i++) {
      left += buffer[i].left * coeffs[i];
      right += buffer[i].right * coeffs[i];
    }

    sample.left = (int16_t) left;
    sample.right = (int16_t) right;
  }

  error_type_t Filter::do_process(audio_buffer_t& buffer) {
    for (auto& sample : buffer.data) process_sample(sample);
    return error_type_t::ok;
  }
}
