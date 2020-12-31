#pragma once
#include "iimavlib.h"

namespace fx_rack {
  using iimavlib::pAudioFilter;
  using iimavlib::AudioFilter;
  using iimavlib::error_type_t;
  using iimavlib::audio_buffer_t;

  class Distortion : public AudioFilter {
    public:
      Distortion(const pAudioFilter& child, double gain, int16_t threshold);
      error_type_t do_process(audio_buffer_t& buffer);

    private:
      const double min = INT16_MIN;
      const double max = INT16_MAX;
      const double gain;
      const int16_t threshold;
      void clip_sample(int16_t& sample);
      void boost_gain(int16_t& sample);
  };
}
