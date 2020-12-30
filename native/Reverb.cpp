#define _USE_MATH_DEFINES
#include "Reverb.h"
#include <math.h>
#include <iostream>
#include <cstdio>

namespace fx_rack {
  Reverb::Reverb(const pAudioFilter &child, double delay, double decay_factor, int mix_percent)
    : AudioFilter(child), delay(delay), decay_factor(decay_factor), mix_percent(mix_percent) {}

  Reverb::~Reverb() {
    if (buffer_initialzied) delete[] _buffer;
  }

  int Reverb::get_buffer_index(int current, int move_by) {
    if (current < 0) return current + move_by;
    int res = current + move_by;
    while (res >= _buffer_size) {
      res -= _buffer_size;
    }
    while (res < 0) {
      res += _buffer_size;
    }
    return res;
  }

  void Reverb::clear_buffer(int size) {
    for (int i = 0; i < size; ++i) {
      _buffer[get_buffer_index(_buffer_w, i)] = 0;
    }
  }

  error_type_t Reverb::do_process(audio_buffer_t &buffer) {
    if (buffer.valid_samples == 0) {
      return error_type_t::ok;
    }
    if (!buffer_initialzied) {
      int buffer_size_ms = delay + 19.31 + 11.73;
      _buffer_size = 1.1 * buffer_size_ms * samp_freq / 1000.0 + 1;
      _buffer_r = - (delay + 11.73) * samp_freq / 1000;
      _buffer = new audio_sample_t[_buffer_size];
      for (int i = 0; i < _buffer_size; ++i) {
        _buffer[i] = 0;
      }
      buffer_initialzied = true;
    }

    clear_buffer(buffer.data.size());
    comb(buffer.data, 0, 0);
    comb(buffer.data, -11.73, -0.1313);
    comb(buffer.data, 19.31, -0.2743);
    comb(buffer.data, -7.97, -0.31);

    for (int i = 0; i < buffer.data.size(); ++i) {
      if (_buffer_r + i >= 0) {
        buffer.data[i] = (1.0 - mix_percent/100.0) * buffer.data[i] + mix_percent/100.0 * _buffer[get_buffer_index(_buffer_r, i)];
      } else {
        buffer.data[i] = (1.0 - mix_percent/100.0) * buffer.data[i];
      }
    }

    _buffer_w = get_buffer_index(_buffer_w, buffer.data.size());
    _buffer_r = get_buffer_index(_buffer_r, buffer.data.size());
    return error_type_t::ok;
  }

  void Reverb::comb(std::vector<audio_sample_t> &samples, double delay_diff, double decay_factor_diff) {
    int delay_samples = delay_diff * samp_freq / 1000;
    for (int i = 0; i < samples.size(); ++i) {
      _buffer[get_buffer_index(_buffer_w, delay_samples + i)] += samples[i] * (decay_factor + decay_factor_diff);
    }
  }
}
