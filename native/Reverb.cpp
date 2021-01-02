#include "Reverb.h"
#include <algorithm>

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

  void Reverb::clear_buffer(size_t size) {
    for (int i = 0; i < size; ++i) {
      _buffer[get_buffer_index(_buffer_w, i)] = 0;
    }
  }

  error_type_t Reverb::do_process(audio_buffer_t &buffer) {
    if (buffer.valid_samples == 0) {
      return error_type_t::ok;
    }
    if (!buffer_initialzied) {
      buffer_initialzied = true;
      int buffer_size_ms = (int) (delay + 19.31 + 11.73 + 89.27);
      _buffer_size =(int) (2 * buffer_size_ms * samp_freq / 1000.0 + 1);
      _buffer_r = (int) (-(delay + 11.73) * samp_freq / 1000);
      _buffer_r_all_pass = (int) (_buffer_r - 89.27 * samp_freq / 1000);
      _buffer = new audio_sample_t[_buffer_size];
      for (int i = 0; i < _buffer_size; ++i) {
        _buffer[i] = audio_sample_t();
      }
    }

    clear_buffer(buffer.valid_samples);
    comb(buffer.data, 0, 0);
    comb(buffer.data, -11.73, -0.1313);
    comb(buffer.data, 19.31, -0.2743);
    comb(buffer.data, -7.97, -0.31);

    for (int i = 0; i < buffer.valid_samples; ++i) {
      if (_buffer_r + i >= 0) {
        _buffer[get_buffer_index(_buffer_r, i)] = (1.0 - mix_percent/150.0) * buffer.data[i] + mix_percent/150.0 * _buffer[get_buffer_index(_buffer_r, i)];
      } else {
        buffer.data[i] *= (1.0 - mix_percent/150.0);
      }
    }

    all_pass(buffer.data);
    all_pass(buffer.data);

    for (int i = 0; i < buffer.valid_samples; ++i) {
      if (get_buffer_index(_buffer_r, i) > 0)
        buffer.data[i] = _buffer[get_buffer_index(_buffer_r, i)];
    }

    _buffer_w = get_buffer_index(_buffer_w, (int) buffer.valid_samples);
    _buffer_r = get_buffer_index(_buffer_r, (int) buffer.valid_samples);
    _buffer_r_all_pass = get_buffer_index(_buffer_r_all_pass, (int)buffer.valid_samples);
    return error_type_t::ok;
  }

  void Reverb::comb(std::vector<audio_sample_t> &samples, double delay_diff, double decay_factor_diff) {
    int delay_samples = (int) (delay_diff * samp_freq / 1000);
    for (int i = 0; i < samples.size(); ++i) {
      safe_add(_buffer[get_buffer_index(_buffer_w, delay_samples + i)], samples[i] * (decay_factor + decay_factor_diff));
    }
  }

  void Reverb::all_pass(std::vector<audio_sample_t> &samples) {
    if (_buffer_r < 0) return;
    double decay_factor_ = 0.131;

    for (int i = 0; i < samples.size(); ++i) {
      if (get_buffer_index(_buffer_r_all_pass, i) >= 0) {
        safe_add(_buffer[get_buffer_index(_buffer_r, i)], - decay_factor_ * _buffer[get_buffer_index(_buffer_r_all_pass, i)]);
        safe_add(_buffer[get_buffer_index(_buffer_r, i)], decay_factor_ * _buffer[get_buffer_index(_buffer_r_all_pass, i + 20)]);
      }
    }
  }

  void Reverb::safe_add(audio_sample_t& sample1, audio_sample_t& sample2) {
      double left = (double) sample1.left + sample2.left;
      double right = (double) sample1.right + sample2.right;
      sample1.left = (int16_t) std::clamp(left, min, max);
      sample1.right = (int16_t) std::clamp(right, min, max);
  }
}
