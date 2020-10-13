#pragma once
#include "iimavlib.h"

namespace sine_gen {
  using iimavlib::AudioFilter;
  using iimavlib::pAudioFilter;
  using iimavlib::error_type_t;
  using iimavlib::audio_buffer_t;

  class SineGen: public AudioFilter {
    public:
      SineGen(const double freq, bool& active);

    private:
      const double freq;
      double time = 0;
      bool& active;
      error_type_t do_process(audio_buffer_t& buffer) override;
  };
}
