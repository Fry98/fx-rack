#pragma once
#include "iimavlib.h"

namespace fx_rack {
  using iimavlib::AudioFilter;
  using iimavlib::pAudioFilter;
  using iimavlib::error_type_t;
  using iimavlib::audio_buffer_t;
  using iimavlib::audio_sample_t;
  using iimavlib::convert_rate_to_int;

  class Reverb : public AudioFilter {
  public:
    Reverb(const pAudioFilter &child, double delay, double decay_factor, int mix_percent);
    error_type_t Reverb::do_process(audio_buffer_t &buffer);
  private:
    double delay;
    double decay_factor;
    int mix_percent;
    double samp_freq = convert_rate_to_int(get_params().rate);
    audio_sample_t* buffer;

    std::vector<audio_sample_t> Reverb::comb(audio_buffer_t &buffer, double delay_diff, double decay_factor_diff);
    std::vector<audio_sample_t> Reverb::all_pass(std::vector<audio_sample_t> &samples);
  };
} // namespace fx_rack
