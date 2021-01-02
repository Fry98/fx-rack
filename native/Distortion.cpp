#include "Distortion.h"
#include <algorithm>

namespace fx_rack {
  Distortion::Distortion(
    const pAudioFilter& child,
    double gain, int16_t threshold
  ): AudioFilter(child), gain(gain), threshold(threshold) {};

  void Distortion::boost_gain(int16_t& sample) {
    double boosted = ((double) sample) * gain;
    sample = (int16_t) std::clamp(boosted, min, max);
  }

  void Distortion::clip_sample(int16_t& sample) {
    int16_t sign = sample != 0 ? sample / abs(sample) : 0;
    if (abs(sample) > threshold) sample = threshold * sign;
  }

  error_type_t Distortion::do_process(audio_buffer_t& buffer) {
    for (auto& sample : buffer.data) {
      boost_gain(sample.left);
      boost_gain(sample.right);
      clip_sample(sample.left);
      clip_sample(sample.right);
    }
    return error_type_t::ok;
  };
}
