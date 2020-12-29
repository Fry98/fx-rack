#define _USE_MATH_DEFINES
#include "Reverb.h"
#include <math.h>
#include <iostream>
#include <cstdio>

namespace fx_rack {
  Reverb::Reverb(const pAudioFilter &child, double delay, double decay_factor, int mix_percent)
    : AudioFilter(child), delay(delay), decay_factor(decay_factor), mix_percent(mix_percent) {
      std::cout << "init" << std::endl;
    }

  error_type_t Reverb::do_process(audio_buffer_t &buffer) {
    if (buffer.valid_samples == 0) {
      return error_type_t::ok;
    }

    std::vector<audio_sample_t> result = comb(buffer, 0, 0);
    std::vector<audio_sample_t> comb_1 = comb(buffer, -11.73, -0.1313);
    std::vector<audio_sample_t> comb_2 = comb(buffer, 19.31, -0.2743);
    std::vector<audio_sample_t> comb_3 = comb(buffer, -7.97, -0.31);

    for (int i = 0; i < buffer.data.size(); ++i) {
      result[i] += comb_1[i] + comb_2[i] + comb_3[i];
    }

    for (int i = 0; i < buffer.data.size(); ++i) {
      result[i] = (1.0 - mix_percent/100.0) * buffer.data[i] + mix_percent/100.0 * result[i];
    }

    result = all_pass(result);
    result = all_pass(result);

    buffer.data = result;
    return error_type_t::ok;
  }

  std::vector<audio_sample_t> Reverb::comb(audio_buffer_t &buffer, double delay_diff, double decay_factor_diff) {
    int delay_samples = (delay + delay_diff) * samp_freq / 1000000.0;
    std::vector<audio_sample_t> comb_samples(buffer.data);
    for (int i = 0; i < comb_samples.size() - delay_samples; ++i) {
      comb_samples[i + delay_samples] += comb_samples[i] * (decay_factor + decay_factor_diff);
    }
    return comb_samples;
  }

  std::vector<audio_sample_t> Reverb::all_pass(std::vector<audio_sample_t> &samples) {
    int delay_samples = 89.27 * samp_freq / 1000000.0;
    double decay_factor_ = 0.131;

    std::vector<audio_sample_t> all_pass_samples(samples);
    for (int i = 0; i < all_pass_samples.size(); ++i) {
      if (delay_samples >= 0 && i >= delay_samples) {
        all_pass_samples[i] += -decay_factor_ * all_pass_samples[i - delay_samples];
      }
      if (delay_samples >= 1 && i + 20 >= delay_samples && i + 20 - delay_samples < all_pass_samples.size()) {
        all_pass_samples[i] += decay_factor_ * all_pass_samples[i + 20 - delay_samples];
      }
    }

    audio_sample_t value = all_pass_samples[0];
    audio_sample_t max = audio_sample_t(0, 0);
    
    for (int i = 0; i < all_pass_samples.size(); ++i) {
      if (std::abs(all_pass_samples[i].left) > max.left || std::abs(all_pass_samples[i].right) > max.right) {
        max = audio_sample_t(std::abs(all_pass_samples[i].left), std::abs(all_pass_samples[i].right));
      }
    }

    // TODO fix clipping below
    if (max.left > 0 && max.right > 0) {
      for (int i = 0; i < all_pass_samples.size(); ++i) {
        value.left = (value.left + (all_pass_samples[i].left - value.left)) / max.left;
        value.right = (value.right + (all_pass_samples[i].right - value.right)) / max.right;
        all_pass_samples[i] = audio_sample_t(value);
        std::cout << value.left << ":" << value.right << std::endl;
      }
    }

    return all_pass_samples;
  }
}
