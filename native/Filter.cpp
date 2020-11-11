#define _USE_MATH_DEFINES
#include "Filter.h"
#include <math.h>

namespace fx_rack {
  Filter::Filter(
    const pAudioFilter& child,
    FilterType type,
    unsigned int num_taps, double cutoff
  ): AudioFilter(child), num_taps(num_taps), cutoff(cutoff), type(type) {
    lambda = M_PI * cutoff / (sampFreq / 2);
    coeffs = new double[num_taps];
    buffer = new audio_sample_t[num_taps];

    for (size_t i = 0; i < num_taps; i++) buffer[i] = 0;
    type == LPF ? computeLPF() : computeHPF();
  }

  Filter::~Filter() {
    delete coeffs;
    delete buffer;
  }

  void Filter::computeLPF() {
    for (size_t i = 0; i < num_taps; i++) {
      double mm = i - (num_taps - 1.0) / 2.0;
      if (mm == 0) coeffs[i] = lambda / M_PI;
      else coeffs[i] = sin(mm * lambda) / (mm * M_PI);
    }
  }

  void Filter::computeHPF() {

  }

  void Filter::process_sample(audio_sample_t& sample) {
    for (size_t i = num_taps - 1; i > 0; i--) {
      buffer[i] = buffer[i - 1];
    }
    buffer[0] = sample;

    audio_sample_t res(0, 0);
    for (size_t i = 0; i < num_taps; i++) {
      res.left += (int16_t) (buffer[i].left * coeffs[i]);
      res.right += (int16_t) (buffer[i].right * coeffs[i]);
    }

    sample.left = res.left;
    sample.right = res.right;
  }

  error_type_t Filter::do_process(audio_buffer_t& buffer) {
    for (auto& sample : buffer.data) process_sample(sample);
    return error_type_t::ok;
  }
}
