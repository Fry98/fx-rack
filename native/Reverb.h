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
    ~Reverb();
    error_type_t Reverb::do_process(audio_buffer_t &buffer);
  private:
    double delay;
    double decay_factor;
    int mix_percent;
    double samp_freq = convert_rate_to_int(get_params().rate);
    int _buffer_size;
    audio_sample_t* _buffer;
    int _buffer_w = 0;
    int _buffer_r;
    int _buffer_r_all_pass;
    bool buffer_initialzied = false;

    void Reverb::comb(std::vector<audio_sample_t> &samples, double delay_diff, double decay_factor_diff);
    void Reverb::all_pass(std::vector<audio_sample_t> &samples);
    int Reverb::get_buffer_index(int current, int move_by);
    void Reverb::clear_buffer(size_t size);
  };
}
