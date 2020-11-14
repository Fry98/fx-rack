#pragma once
#include "iimavlib.h"

namespace fx_rack {
  using iimavlib::pAudioFilter;
  using iimavlib::AudioFilter;
  using iimavlib::convert_rate_to_int;
  using iimavlib::error_type_t;
  using iimavlib::audio_buffer_t;
  using iimavlib::audio_sample_t;

  enum FilterType {
    LPF,
    HPF
  };

  class Filter : public AudioFilter {
    public:
      Filter(const pAudioFilter& child, FilterType type, unsigned int num_taps, double cutoff);
      ~Filter();
      error_type_t Filter::do_process(audio_buffer_t& buffer);

    private:
      unsigned int num_taps;
      FilterType type;
      double cutoff;
      double sampFreq = convert_rate_to_int(get_params().rate);
      double* coeffs;
      audio_sample_t* buffer;
      void computeLPF();
      void computeHPF();
      void process_sample(audio_sample_t& sample);
  };
}
